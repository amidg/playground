#include <iostream>
#include <cstddef>
#include <optional>
#include <array>
#include <atomic>
#include <chrono>
#include <thread>

using namespace std::chrono_literals;

template <typename T, size_t Size>
class SPSCRingBuffer {
    static_assert(Size % 2 == 0, "must be even number");
    static_assert(Size && (Size & (Size - 1)) == 0, "must be power of two");
    static constexpr size_t BufferMask = Size - 1;
public:
    SPSCRingBuffer() : size_(Size) { }

    // TODO:
    // Think about making it non-movable and non-copyable
    // Do I need to follow rule of 0/3/5

    [[nodiscard]] bool push(const T& item) {
        const size_t head = new_push_idx_.load(std::memory_order_relaxed);
        const size_t tail = next_read_idx_.load(std::memory_order_acquire);

        if ((head - tail) >= size_) {
            //printf("Buffer is full (head / tail): %ld / %ld\n", head, tail);
            return false;
        }
       
        data_[head & BufferMask] = item;
        new_push_idx_.store(head + 1, std::memory_order_release);
        //std::cout << "Pushed at " << push_idx << std::endl;
        return true;
    }

    [[nodiscard]] std::optional<T> pop() {
        const size_t head = new_push_idx_.load(std::memory_order_acquire);
        const size_t tail = next_read_idx_.load(std::memory_order_relaxed);

        std::optional<T> result = std::nullopt;

        if (head == tail) {
            //std::cerr << "buffer is empty" <<std::endl;
            return result;
        }

        result = data_[tail & BufferMask];
        next_read_idx_.store(tail + 1, std::memory_order_release);
        return result;
    }

    [[nodiscard]] size_t size() const {
        size_t tail = next_read_idx_.load(std::memory_order_relaxed);
        size_t head = new_push_idx_.load(std::memory_order_relaxed);
        size_t size = head - tail;
        std::cout << size << std::endl;
        return size;
    }

    void clear() {
        new_push_idx_ = 0;
        next_read_idx_ = 0;
    }

private:
    const size_t size_;
    std::array<T, Size> data_;
    // avoiding false-sharing on Intel processors, as those may pre-fetch two cache lines at a time, rather than one.
    // Every variable we manipulate in the atomic manner must be aligned to avoid error and allow maximum throughput
    alignas(std::hardware_destructive_interference_size) std::atomic<size_t> new_push_idx_{0};
    alignas(std::hardware_destructive_interference_size) std::atomic<size_t> next_read_idx_{0};
};


int main(/*int argc, char * argv[]*/) {
    constexpr size_t size = 100'000;
    constexpr size_t buffer_size = 1024; // N
    SPSCRingBuffer<size_t, buffer_size> buffer;

    // Test 1 -> push N times / pop N times
    for (size_t i = 0; i < size; i++) {
        if (!buffer.push(i)) {
            std::cerr << "Test 1 FAIL: fail push at " << i << std::endl;
            return 1;
        }

        std::optional<size_t> test_i = buffer.pop();
        if (test_i == std::nullopt) {
            std::cerr << "Test 1 FAIL: nullopt at index " << i << std::endl;
            return 1;
        } else if (i != test_i.value()) {
            std::cerr << "Test 1 FAIL: incorrect order at index " << i << std::endl;
            return 1;
        }
    }

    std::cout << "Test 1 PASS" << std::endl;

    // Test 2 -> push N+1 times
    buffer.clear();
    for (size_t i = 0; i < buffer_size; i++) {
        if (!buffer.push(i)) {
            std::cerr << "Test 2 FAIL: fail push at " << i << std::endl;
            return 1;
        }
    }
    if (!buffer.push(buffer_size+1))
        std::cout << "Test 2 PASS: fail to push at N+1" << std::endl;

    // Multithreaded Test 3
    buffer.clear();
    std::thread imu_thread_(
        [&buffer](){
             size_t i = 0;
             while (i < size) {
                 if (buffer.push(i)) {
                     //printf("[IMU] Published: %ld\n", i);
                     i++;
                 } else {
                     continue;
                 }
             }
             printf("Publisher is done\n");
        });

    std::thread consumer_thread_(
        [&buffer](){
            size_t last_value = -1;
            std::optional<size_t> val;
            size_t i = 0;
            while (i < size) {
                val = buffer.pop();
                if (val == std::nullopt) continue;

                const size_t value = val.value();
                if (value - last_value == 1) {
                    last_value = value;
                    //printf("[Consumer] received: %ld\n", value);
                    i++;
                } else {
                    printf(
                        "Got wrong value. Expected %ld / got %ld\n",
                        last_value+1, value);
                    break;
                }
            }
        });

    imu_thread_.join();
    consumer_thread_.join();

    return 0;
}

