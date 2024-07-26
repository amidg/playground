#include <atomic>
#include <iostream>
#include <thread>

std::atomic<int> count(0);

void increment() {
    for (int i = 0; i < 10000; ++i) {
        count.fetch_add(1, std::memory_order_relaxed);
    }
}

int main() {
    std::thread t1(increment);
    std::thread t2(increment);

    t1.join();
    t2.join();

    std::cout << "Count: " << count << '\n';
    return 0;
}

