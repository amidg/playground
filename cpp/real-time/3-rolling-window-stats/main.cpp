#include <iostream>
#include <array>
#include <algorithm>
#include <optional>
#include <stdexcept>
#include <atomic>

template<typename T, size_t Size>
class RollingMedian {
public:
    RollingMedian() : size_(Size), count_(0) {
        data_.fill(T()); // will always fill with 0.0
    };

    RollingMedian(const RollingMedian&) = delete;
    RollingMedian& operator=(const RollingMedian&) = delete;

    // should we also delete moving constructor and moving assignment operator?

    void push(T value) noexcept {
        // this will automatically wrap the whole thing
        size_t idx = head.load(std::memory_order_acquire);
        data_[idx] = value;
        idx = ((idx + 1) < size_) ? idx + 1 : 0;
        count_++; // does not need to be capped at Size because what matters is >0
        head.store(idx, std::memory_order_release);
    }

    std::optional<T> median() noexcept {
        std::optional<T> result = std::nullopt;
        
        if (count_ == 0) return result;

        std::array<T, Size> data_copy = data_;
        std::sort(data_copy.begin(), data_copy.end());
        const size_t midIdx = size_ / 2;

        if (size_ % 2 == 0) {
            const size_t beforeMidIdx = midIdx - 1;
            result =
                (data_copy[midIdx] > data_copy[beforeMidIdx])
                ? data_copy[beforeMidIdx]
                : data_copy[midIdx];
        } else {
            result = static_cast<T>(data_copy[midIdx]);
        }
        return result;
    }

    std::optional<T> peek(size_t idx) noexcept {
        if (idx >= size_)
            return std::nullopt;
        return data_[idx];
    }

private:
    size_t size_;
    size_t count_;
    std::array<T, Size> data_;
    alignas(std::hardware_destructive_interference_size) std::atomic<size_t> head{0};
};

int main(/*int argc, char * argv[]*/) {
    constexpr size_t size = 64;
    RollingMedian<double, size> window;

    // Test 1 - Pushing
    for (size_t i = 0; i < size; i++) {
        if (i == 0)
            window.push(100);
        else
            window.push(i);
    }
    double test_1_val = window.peek(0).value();
    if (test_1_val == 100)
        printf("Test 1 PASS\n");
    else
        printf("Test 1 FAIL\n");

    // Test 2 - Pushing N+1 with outlier
    const double test = 22.8;
    window.push(test);
    if (window.peek(0) == test)
        std::cout << "Test 2 PASS: outlier 100 is gone" << std::endl;
    else
        std::cout << "Test 2 FAIL: does not match N+1" << std::endl;

    // Test 3 - size 1
    RollingMedian<double, 1> window_1;
    window_1.push(3.0);
    const auto test3 = window_1.median();
    if (test3 == std::nullopt)
        std::cout << "Test 3 FAIL" << std::endl;
    else if (test3.value() == 3.0)
        std::cout << "Test 3 PASS" << std::endl;
    else
        std::cout << "Test 3 FAIL: " << test3.value() << std::endl;

    // Test 4 - even size
    RollingMedian<double, 4> window_2;
    window_2.push(1);
    window_2.push(3);
    window_2.push(2);
    window_2.push(4);
    const auto test4 = window_2.median();
    if (test4 == std::nullopt)
        std::cout << "Test 4 FAIL" << std::endl;
    else if (test4.value() == 2.0)
        std::cout << "Test 4 PASS" << std::endl;
    else
        std::cout << "Test 4 FAIL: " << test4.value() << std::endl;

    return 0;
}

