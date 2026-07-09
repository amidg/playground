#include <iostream>
#include <array>
#include <algorithm>
#include <optional>
#include <stdexcept>
#include <atomic>
#include <thread>
#include <chrono>

using namespace std::chrono_literals;

#define NUM_SIGNALS 5

struct SignalConfig {
    float warning_threshold;
    float fault_threshold;
};

enum class SystemState : uint8_t { NOMINAL, WARNING, FAULT };

// NOTE (dmitrii): regarding the weak VS strong CAS
// x86 macihnes:
// - compile to the same instruction `lock cmpxchg`
// - true hardware atomic operations
// RISC machine (load-linked / store-conditional architectures, e.g. ARM):
// - compare_exchange_strong compiles to a retry loop that masks spurious failures
// - compare_exchange_weak compiles to a single LDREX/STREX pair with no retry 
// - spurious failure can completel skip CAS
// To make code hardware agnostic, code can be wrapped into a loop

template <std::size_t N>
class HardwareMonitor {
private:
    // signal data
    std::array<SignalConfig, N> configs_;
    std::array<float, N> readings_;
    // we need to ensure that we avoid dangerous situation while initializing
    // before first run of update()
    std::atomic<SystemState> state_{SystemState::NOMINAL};
public:
    explicit HardwareMonitor(const std::array<SignalConfig, N>& configs) noexcept
    : configs_(configs) {
        // NOTE (dmitrii):
        // in theory there should be some kind of safety logic here that prevents
        // state_ from being NOMINAL by default, but this is not outlined in the task
    }

    // copy constructor and copy assignment operator are delete
    HardwareMonitor (const HardwareMonitor&) = delete;
    HardwareMonitor& operator=(const HardwareMonitor&) = delete;

    // rule of 5:
    // - we have custom destructor
    // - deleted copy constructor and copy assignment 
    // - also delete the moving constructor and move assignment operator
    HardwareMonitor (const HardwareMonitor&&) = delete;
    HardwareMonitor& operator=(const HardwareMonitor&&) = delete;

    // Similar idea to the SPSCRingBuffer example, this is the matter of ownership
    // update is the only one that actually owns the state_ variable

    // Called from the RT thread at 1kHz. Must never throw. Must never allocate.
    // NOTE (dmitrii): update must do the following:
    // - Safely write all the values without additional allocations
    // - Correcty handle the compare_exchange of the state_ variable
    void update(const std::array<float, N>& readings) noexcept {
        SystemState temp{SystemState::NOMINAL};
        for (size_t i = 0; i < N; i++) {
            // NOTE (dmitrii):
            // okay to do this way because no other thread here reads the states
            // logic can be simplified and does not require any kind of locking
            readings_[i] = readings[i];

            // validate values
            if (readings_[i] > configs_[i].warning_threshold)
                temp = SystemState::WARNING;
            
            if (readings_[i] > configs_[i].fault_threshold)
                temp = SystemState::FAULT;
        }

        // NOTE (dmitrii):
        // correctly handle the system state machine
        // control loop thread must never do heap allocation
        auto expected_state = state_.load(std::memory_order_acquire);
        if (expected_state == SystemState::FAULT)
            return;
        if (state_.compare_exchange_weak(
                expected_state,           // expected
                temp,                      // desired
                std::memory_order_release, // if success
                std::memory_order_relaxed  // if failure
            )) {
            // NOTE (dmitrii):
            // printf() inside the RT function is bad
            // ideally this should be a ring buffer
            printf(
                "Current: %d\nExpected: %d\n",
                static_cast<uint8_t>(state_.load()),
                static_cast<uint8_t>(expected_state)
            );
        }
    }

    // Safe to call from any thread at any time.
    [[nodiscard]] SystemState state() const noexcept {
        SystemState result = state_.load(std::memory_order_acquire);
        return result;
    }

    // Resets a latched FAULT back to NOMINAL. Call only after the fault condition
    // has been confirmed cleared by the operator.
    void reset() noexcept {
        // NOTE (dmitrii)
        // we should use compare_exchange_strong here because correctness is required
        // logic here will use internal retries inside the strong
        // commit to state_ only if nobody internally rewrites this
        // it uses seq_cst ordering as the default one
        auto expected_state = state_.load(std::memory_order_acquire);
        if (state_.compare_exchange_strong(expected_state, SystemState::NOMINAL)) {
            printf(
                "Current: %d\nExpected: %d\n",
                static_cast<uint8_t>(state_.load()),
                static_cast<uint8_t>(expected_state)
            );
        }
    }

    // mock of the power disabled
    // TODO: ideally should have a bunch of wrapper safety logic inside
    // simple function for now because this code base tests different concepts
    void disable_actuator_power() {
        printf("Stopped actuators\n");
    }

    // Destructor must execute a safe shutdown sequence:
    //   1. Transition state to FAULT
    //   2. Disable power to actuators (call disable_actuator_power() or mock it)
    // Order matters — do not disable power before setting the fault state.
    ~HardwareMonitor() {
        // step 1: transition state to Fault
        state_.store(SystemState::FAULT, std::memory_order_release);
        printf("State is set to FAULT, initiating shutdown sequence\n");

        // step 2: disable power to actuators
        disable_actuator_power();
    }
};

int main(/*int argc, char * argv[]*/) {
    constexpr std::array<SignalConfig, NUM_SIGNALS> signals = {
        SignalConfig{1.0, 2.0}, // signal 1
        SignalConfig{2.0, 3.0}, // signal 2
        SignalConfig{3.0, 4.0}, // signal 3
        SignalConfig{4.0, 5.0}, // signal 4
        SignalConfig{5.0, 6.0}  // signal 5
    };
    
    HardwareMonitor<NUM_SIGNALS> hw = HardwareMonitor(signals);

    // test architecture
    // this thread (main) -> real-time, to control the update()
    // remote thread is monitoring thread or reset thread
    std::thread monitor = std::thread(
        [&hw](){
            SystemState result{SystemState::NOMINAL};
            bool is_new{false};
            bool is_done{false};
            while (!is_done) {
                SystemState new_state = hw.state();
                is_new = (new_state != result);

                if (is_new) {
                    switch (new_state) {
                        case SystemState::NOMINAL:
                            printf("System is OK\n");
                            if (result == SystemState::FAULT) {
                                printf("Test 4 - OK\n");
                                is_done = true; // end of test cases
                            }
                            break;
                        case SystemState::WARNING:
                            printf("System is WARNING\n");
                            if (result == SystemState::NOMINAL)
                                printf("Test 1 - OK\n");
                            break;
                        case SystemState::FAULT:
                            printf("System is FAULT\n");
                            if (result == SystemState::WARNING ||
                                result == SystemState::NOMINAL)
                                printf("Test 2 - OK\n");
                            break;
                    }
                    is_new = false;
                    result = new_state;
                }
            }
        }
    );

    // test 1 has passed if shows WARNING
    std::this_thread::sleep_for(2000ms);
    printf("test 1: set to warning\n");
    constexpr std::array<float, NUM_SIGNALS> warning_levels = {1.5, 1.5, 1.5, 1.5, 1.5}; 
    hw.update(warning_levels);

    // test 2 has passed if shows FAULT
    std::this_thread::sleep_for(2000ms);
    printf("test 2: set to fault\n");
    constexpr std::array<float, NUM_SIGNALS> fault_levels = {2.5, 1.5, 1.5, 1.5, 1.5}; 
    hw.update(fault_levels);

    // test 3 update called with nominal values while in FAULT still returns FAULT
    std::this_thread::sleep_for(2000ms);
    printf("test 3: set to nominal values\n");
    constexpr std::array<float, NUM_SIGNALS> nominal = {0.5, 0.5, 0.5, 0.5, 0.5}; 
    hw.update(nominal);
    if (hw.state() == SystemState::FAULT)
        printf("Test 3 - OK\n");

    // test 4: reset state
    std::this_thread::sleep_for(2000ms);
    printf("test 4: reset state\n");
    hw.reset();
    
    monitor.join();

    return 0;
}

