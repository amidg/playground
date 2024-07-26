#include <iostream>
#include <thread>
#include <mutex>

std::mutex mtx;
void print_block(int n, char c) {
    mtx.lock();
    for (int i = 0; i < n; ++i) { std::cout << c; }
    std::cout << '\n';
    mtx.unlock();
}

int main() {
    std::thread t1(print_block, 50, '*');
    std::thread t2(print_block, 50, '$');

    t1.join();
    t2.join();

    return 0;
}

