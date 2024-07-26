#include <iostream>
#include <thread>
#include <semaphore>
#include <vector>

std::counting_semaphore<2> sem(2);  // Initialize semaphore with 2 slots

void worker(int id) {
    sem.acquire();  // Wait for the semaphore
    std::cout << "Worker " << id << " is running\n";
    std::this_thread::sleep_for(std::chrono::seconds(1));  // Simulate work
    std::cout << "Worker " << id << " is done\n";
    sem.release();  // Release the semaphore
}

int main() {
    std::vector<std::thread> workers;
    for (int i = 0; i < 6; i++) {
        workers.emplace_back(worker, i);
    }

    for (auto& t : workers) {
        t.join();
    }

    return 0;
}

