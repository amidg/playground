#include <iostream>
#include <set>
#include <stdexcept>
#include <utility>

// public std::set
// stores sorted set of handles
std::set<int> handles_;
std::set<int> enabled_;

// NOTE (dmitrii):
// task says these are raw C handles, but at the same time expects them to use
// global std::set which is C++ feature
// This task validates RAII princinple, so std::set will be used internally
namespace MockMotorSDK {
    // Returns handle id >= 0 on success, -1 on failure
    int  open_controller(int axis_id) {
        if (axis_id < 0) {
            printf("Invalid handle for the controller\n");
            return -1;
        }
        return axis_id;
    }
    // Must be called before enable
    bool configure(int handle, double max_torque_nm) {
        if (handles_.contains(handle)) {
            printf("Handle already exists. Nothing to configure\n");
            return false;
        }
        handles_.insert(handle);
        printf(
            "MotorController is configured with ID %d / Torque %f\n",
            handle, max_torque_nm);
        return true;
    }
    // Begins torque output
    bool enable(int handle) {
        if (!handles_.contains(handle)) {
            printf("Handle does not exist\n");
            return false;
        } else if (enabled_.contains(handle)) {
            printf("Handle is already enabled\n");
            return false;
        }
        enabled_.insert(handle);
        printf("Handle has been enabled\n");
        return true;
    }
    // Stops torque output — must be called before close if enabled
    void disable(int handle) {
        enabled_.erase(handle);
        printf("Handle was disabled\n");
    }
    // Releases handle — must be called before process exits
    void close_controller(int handle) {
        handles_.erase(handle);
        printf("Handle was closed\n");
    }
}

class MotorController {
private:
    int id_{-1};

public:
    explicit MotorController(int id) : id_(id) {
        int result = MockMotorSDK::open_controller(id_);
        if (result == -1)
            throw std::runtime_error("Failed to open controller\n");
        if (!MockMotorSDK::configure(id_, 0)) {
            // add the close_controller before throwing in case it acquires kernel resources
            MockMotorSDK::close_controller(id_);
            id_ = -1;
            throw std::runtime_error("Failed to configure controller\n");
        }
    }

    // destructor
    ~MotorController() noexcept {
        // disable first
        if (is_enabled())
            MockMotorSDK::disable(id_);

        // check if present in handles to avoid double close
        if (handles_.contains(id_))
            MockMotorSDK::close_controller(id_);
    }

    // not copyable
    MotorController (const MotorController&) = delete;
    MotorController& operator=(const MotorController&) = delete;

    // movable
    MotorController (MotorController&& other) noexcept
        : id_(std::exchange(other.id_, -1)) {}

    MotorController& operator=(MotorController&& other) noexcept {
        if (this == &other)
            return *this;
        if (id_ >= 0) {
            if (is_enabled())
                MockMotorSDK::disable(id_);
            MockMotorSDK::close_controller(id_);
        }
        id_ = std::exchange(other.id_, -1);
        return *this;
    }

    // enable
    void enable() {
        if (!MockMotorSDK::enable(id_))
            throw std::runtime_error("Failed to enable controller\n");
    }

    // getter
    [[nodiscard]] bool is_enabled() const noexcept {
        return enabled_.contains(id_);
    }
};


int main(/*int argc, char * argv[]*/) {
    // Test 1: normal
    // Uses scope guard to test RAII
    printf("Test 1: normal\n");
    {
        MotorController mc(0);
        mc.enable();
    }  // destructor fires here — prints disable then close
    printf("Test 1 PASS\n");

    // Test 2: throw on open
    printf("Test 2: throw on open\n");
    MotorController mc(1);
    try {
        MotorController mc_new(1);
    } catch (std::exception& e) {
        std::cout << "Test 2 OK: " << e.what() << std::endl;
        // mc handle is closed, but mc_new does not call destructor
    }

    // Test 3: throw on enable
    printf("Test 3: throw on enable\n");
    MotorController mc3(1);
    mc3.enable();
    try {
        mc3.enable();
    } catch (std::exception& e) {
        std::cout << "Test 3 OK: " << e.what() << std::endl;
    }

    // Test 4: move
    // should not report any stdout
    printf("Test 4: move semantics\n");
    MotorController mc_moved = std::move(mc3);
    printf("Test 4: DONE. OK if nothing reported in between\n");

    // Test 5: move assignment operator
    printf("Test 5: move assignment operator\n");
    MotorController mc_a(2);
    MotorController mc_b(3);
    mc_a = std::move(mc_b);
    printf("Test 5: PASS if one handle was closed\n");

    return 0;
}

