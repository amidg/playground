#include <iostream>
#include <vector>
#include <future>
#include <mutex>
#include <condition_variable>
#include <thread>

// custom
#include "input.h"
#include "house.h"

std::mutex mtx;
std::condition_variable cv;
std::vector<int> shared_vector;
int current_number = 0;
bool done = false; // Flag to signal when to stop the threads

// Function to append even numbers
void append_even_numbers() {
    while (true) {
        std::unique_lock<std::mutex> lock(mtx);
        cv.wait(lock, [] { return done || (current_number % 2 == 0); });

        if (done) break; // Exit if done

        shared_vector.push_back(current_number);
        ++current_number;
        cv.notify_all(); // Notify other threads
    }
}

// Function to append odd numbers
void append_odd_numbers() {
    while (true) {
        std::unique_lock<std::mutex> lock(mtx);
        cv.wait(lock, [] { return done || (current_number % 2 == 1); });

        if (done) break; // Exit if done

        shared_vector.push_back(current_number);
        ++current_number;
        cv.notify_all(); // Notify other threads
    }
}

int main() {
    // Launch asynchronous tasks
    std::future<void> even_future = std::async(std::launch::async, append_even_numbers);
    std::future<void> odd_future = std::async(std::launch::async, append_odd_numbers);

    // Wait until 100 numbers have been appended
    while (true) {
        std::unique_lock<std::mutex> lock(mtx);
        if (shared_vector.size() >= 100) {
            done = true;
            cv.notify_all(); // Notify threads to stop
            break;
        }
    }

    // Wait for tasks to complete
    even_future.get();
    odd_future.get();

    // Output the results
    for (int v : shared_vector) {
        std::cout << v << " ";
    }
    std::cout << std::endl;

    return 0;
}
