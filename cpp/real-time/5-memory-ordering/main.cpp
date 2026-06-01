#include <iostream>
#include <thread>
#include <vector>
#include <string>
#include <mutex>
#include <chrono>
#include <atomic>

using namespace std::chrono_literals;

// This task does not provide any logic to work with, thus I built my own below
// This logic cannot be altered
// Only order of mutex locking may be changed as stated in the part B
// Control Thread -> holds joint mutex to write, waits for sensor mutex to read sensors
// Sensor Thread -> holds sensor mutex to write values, waits for joint mutex to write values
struct Sensor {
    std::mutex mtx_;
    float value_;
};

/* 
Part A: Problem Identification

EXPLANATION
This is a clear circular wait case that stems from the Coffman's Four Deadlock Conditions. Coffman's deadlock condition means four cases happen simultaneously:
1. mutual exclusion <- each mutex can be held by one thread at a time
2. hold and wait <- both threads hold mutexes without releasing them
3. no preemption <- this code does not provide any mechanism for the OS to release the resource
4. circular wait <- wait for each other

In this case we can clearly see that both control and sensor threads are stuck in the deadlock condition that looks like a circular wait. This happens due to the following possible reasons:
- either because both threads have same priority or priority inversion case occurs
- Neither thread checks the ability to lock mutex before acquiring it (e.g. what std::unique_lock does)
- Neither thread has a timeout
*/

// Part A: example of how this works and fails at deadlock
// Due to 1s delay this is statistically unlikely as it will deadlock inconsistently under the load due to the following working order
// 1. Lock the opposite threads's lock
// 1. Lock the necessary lock
// 2. do work
// 3. Unlock both ones in the opposite order
// This deadlock condition can be masked in the following ways:
// - Timing (even 1ms) allows to introduce inconsistent periods of availability (e.g. std::this_thread::sleep_for(1ms))
// - try_lock() allows to prevent deadlock, however it is not scalable. As number of required mutexes grows this approach will create unscalabe code and does not allow for fair share of CPU reesources
void A_controls_func(Sensor& joint, Sensor& sensor, std::vector<std::string>& sources) {
    while(true) {
        std::cout << "Controls waiting for lock" << std::endl;
        sensor.mtx_.lock();
        joint.mtx_.lock();
        for (const auto& name : sources) {
            if (name.find("joint") != std::string::npos)
                std::cout << "joint: " << name << std::endl;
        }
        joint.mtx_.unlock();
        sensor.mtx_.unlock();
    }
}

void A_sensor_func(Sensor& joint, Sensor& sensor, std::vector<std::string>& sources) {
    while(true) {
        std::cout << "Sensor waiting for lock" << std::endl;
        joint.mtx_.lock();
        sensor.mtx_.lock();
        for (const auto& name : sources) {
            if (name.find("ft") != std::string::npos)
                std::cout << "sensor: " << name << std::endl;
        }
        sensor.mtx_.unlock();
        joint.mtx_.unlock();
    }
}

// Part B
// EXPLANATION
// - std::scoped_lock is a RAII wrapper that allows to take ownership of multiple mutexes in the atomic manner, order of mutexes does not matter
// - std::adopt_lock is the flag that indicates that we assume the calling thread already has ownership of the mutex 
// - Code below has a problem that both threads do not share CPU time equally because they lack conditional variable or some sort of alternating flag (e.g. using atomic)
// Approach 1: acquire multiple locks using std::scoped_lock
void b1_read_value(Sensor& j, Sensor& s, std::vector<std::string>& sources, std::string type) {
    while (true) {
        std::cout << type << " is waiting for lock" << std::endl;
        std::scoped_lock lock(j.mtx_, s.mtx_);
        std::cout << type << " got lock" << std::endl;
        for (const auto& name : sources) {
            if (name.find(type) != std::string::npos)
                std::cout << type << ": " << name << std::endl;
        }
    }
}

// Approach 2: use std::adopt_lock
// std::adopt_lock assumes that both mutexes are already locked, thus locking will be skipped by the constructor of the RAII lock object (e.g. lock_guard).
// Default behavior is to lock immediately
// Removing lock() before calling lock_guard(..., std::adopt_lock) will cause undefined behavior on unlocking because there is nothing locked in the first place. In this particular case it will look like gibberish stdout
void b2_read_value(Sensor& j, Sensor& s, std::vector<std::string>& sources, std::string type) {
    while (true) {
        std::cout << type << " is waiting for lock" << std::endl;
        std::lock(j.mtx_, s.mtx_);
        std::lock_guard<std::mutex> lk1(j.mtx_, std::adopt_lock);
        std::lock_guard<std::mutex> lk2(s.mtx_, std::adopt_lock);
        std::cout << type << " got lock" << std::endl;
        for (const auto& name : sources) {
            if (name.find(type) != std::string::npos)
                std::cout << type << ": " << name << std::endl;
        }
    }
}

/* Part C
EXPLANATION
This class follows Rule of Three and Rule of Five in C++:
- It defines custom destructor and Copy Constructor / Copy Assignment operator are custom (deleted manually)
- Because Destructor/Copy Constructor/Copy Assignment Operator are defined (deleted) we also need to define Move Constructor / Move Assignment Operator, in this case delete them as well

This is RAII pattern, thus Resource Allocation (private mutexes) Is Initialization (should lock)

Address ordering allows to solve the circular wait problem because in this case it will lock the correct order. The address is just a convenient pre-existing total order. You could use mutex IDs, names, or any other globally consistent ranking. The address works because it is unique by construction, available at runtime with no overhead, and requires no registration or shared bookkeeping.
*/
class OrderedLock {
private:
    std::unique_lock<std::mutex> lock_a_;
    std::unique_lock<std::mutex> lock_b_;
public:
    OrderedLock() = delete;

    OrderedLock(std::mutex& a, std::mutex& b)
        : lock_a_(a, std::defer_lock), lock_b_(b, std::defer_lock) {
        if (&a < &b) {
            lock_a_.lock();
            lock_b_.lock();
        } else if (&a > &b) {
            lock_b_.lock();
            lock_a_.lock();
        } else if (&a == &b) {
            return;
        }
    }

    // must not double unlock
    ~OrderedLock() {
        if (lock_b_.owns_lock()) lock_b_.unlock();
        if (lock_a_.owns_lock()) lock_a_.unlock();
    };

    // non-copyable and non-movable to prevent multiple owners
    OrderedLock(const OrderedLock&) = delete;
    OrderedLock& operator=(const OrderedLock&) = delete;
    OrderedLock(OrderedLock&&) = delete;
    OrderedLock& operator=(OrderedLock&&) = delete;
};

void c_func(
    Sensor& acq, Sensor& hld, std::vector<std::string>& sources, std::string type) {
    while (true) {
        std::cout << type << " thread is waiting for lock" << std::endl;
        OrderedLock lock(acq.mtx_, hld.mtx_);
        std::cout << type << " got lock" << std::endl;
        for (const auto& name : sources) {
            if (name.find(type) != std::string::npos)
                std::cout << type << ": " << name << std::endl;
        }
    }
}


int main(/*int argc, char * argv[]*/) {
    // Considering this is robot's control system it talks to the hardware.
    // Let's assume this IO-bound task thus correct mutex handling is required.
    // Vector below represents the IO-bound problem
    // both threads need to access same data simultaneously
    // joint = encoder reading (joint)
    // ft = force-torque reading (sensor)
    Sensor encoder, ft;
    std::vector<std::string> sources = {
        "r_arm1_joint", "l_arm1_joint", "r_arm1_ft", "l_arm1_ft"};

    // part A - show how this problem might work
    // commented out by default to allow solutions B and C to work
    //std::thread controls_thread_([&](){ A_controls_func(encoder, ft, sources); });
    //std::thread sensor_thread_([&](){ A_sensor_func(encoder, ft, sources); });

    // part B
    // sensor thread is producer
    // controls thread is consumer
    // Approach 1
    //std::thread sensor_thread_([&](){ b1_read_value(ft, encoder, sources, "ft"); });
    //std::thread controls_thread_([&](){ b1_read_value(encoder, ft, sources, "joint"); });
    // Approach 2
    //std::thread sensor_thread_([&](){ b2_read_value(ft, encoder, sources, "ft"); });
    //std::thread controls_thread_([&](){ b2_read_value(encoder, ft, sources, "joint"); });

    // Part C
    std::thread sensor_thread_([&](){ c_func(ft, encoder, sources, "ft"); });
    std::thread controls_thread_([&](){ c_func(encoder, ft, sources, "joint"); });

    // wait for all threads to finish
    controls_thread_.join();
    sensor_thread_.join();

    return 0;
}

