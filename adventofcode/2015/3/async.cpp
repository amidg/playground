#include <iostream>
#include <vector>
#include <future>
#include <mutex>
#include <condition_variable>

// custom
#include "input.h"
#include "house.h"

std::mutex mtx;
std::condition_variable cv;
std::vector<House> houses;
int idx = 0;
bool done = false; // Flag to signal when to stop the threads
                   
// initial data
std::pair<int, int> santa_xy {0, 0};
std::pair<int, int> robot_xy = santa_xy;

// support functions
void calculate_move(std::pair<int, int>& xy, char cmd) {
    switch (cmd) {
        case NORTH:
            ++xy.second;
            break;
        case SOUTH:
            --xy.second;
            break;
        case EAST:
            ++xy.first;
            break;
        case WEST:
            --xy.first;
            break;
    }
}

void calculate_house(std::pair<int, int>& xy) {
    House temp(xy);
    std::vector<House>::iterator house_iter;
    if ( (house_iter = std::find(houses.begin(), houses.end(), temp)) != houses.end()) {
        house_iter->add_gift();
    } else {
        houses.push_back(temp);
    }
}

// async functions
void move_santa() {
    while (true) {
        std::unique_lock<std::mutex> lock(mtx);
        cv.wait(lock, []{ return done || (idx % 2 == 0); });
        if (done) break;
        //std::cout << "santa @ " << idx << std::endl;
        calculate_move(santa_xy, commands[idx]);
        calculate_house(santa_xy);
        ++idx;
        cv.notify_all();
    }
}

//void move_robot(std::pair<int, int>& xy) {
void move_robot() {
    while (true) {
        std::unique_lock<std::mutex> lock(mtx);
        cv.wait(lock, []{ return done || (idx % 2 == 1); });
        if (done) break;
        //std::cout << "robot @ " << idx << std::endl;
        calculate_move(robot_xy, commands[idx]);
        calculate_house(robot_xy);
        ++idx;
        cv.notify_all();
    }
}

int main() {
    // houses
    houses.push_back(House(santa_xy));
    houses.begin()->add_gift();

    // Launch asynchronous tasks
    std::future<void> santa_fut = std::async(
        std::launch::async,
        move_santa//,
        //std::ref(santa_xy)
    );

    std::future<void> robot_fut = std::async(
        std::launch::async,
        move_robot//,
        //std::ref(robot_xy)
    );

    while(true) {
        std::unique_lock<std::mutex> lock(mtx);
        if (idx >= commands.size()) {
            done = true;
            cv.notify_all();
            break;
        }
    }

    santa_fut.get();
    robot_fut.get();

    // check how many houses have more than one gift
    int result = 0;
    for (auto& house : houses) {
        if (house.get_gifts() >= 1)
            ++result;
    }

    std::cout << result << std::endl;

    return 0;
}
