#include <iostream>
#include <vector>

// custom
#include "input.h"
#include "house.h"

int main() {
    std::pair<int, int> santa_xy {0, 0};
    std::pair<int, int> robot_xy = santa_xy;
    std::vector<House> houses; 

    // iterate over the elf commands
    House temp(santa_xy);
    std::vector<House>::iterator found_house;
    houses.push_back(temp); // automatically add 1 gift
    houses.begin()->add_gift(); // add 1 gift for the robot
    bool is_even = false;
    for (int i = 0; i < commands.size(); i++) {
        // read command
        is_even = i%2==0;
        switch (commands[i]) {
            case NORTH:
                if (is_even)
                    ++santa_xy.second;
                else
                    ++robot_xy.second;
                break;
            case SOUTH:
                if (is_even)
                    --santa_xy.second;
                else
                    --robot_xy.second;
                break;
            case EAST:
                if (is_even)
                    ++santa_xy.first;
                else
                    ++robot_xy.first;
                break;
            case WEST:
                if (is_even)
                    --santa_xy.first;
                else
                    --robot_xy.first;
                break;
        }

        // add house
        if (is_even)
            temp.apply_coordinates(santa_xy);
        else
            temp.apply_coordinates(robot_xy);
        if ( (found_house = std::find(houses.begin(), houses.end(), temp)) != houses.end()) {
            found_house->add_gift();
        } else {
            houses.push_back(temp);
        }
    }

    // check how many houses have more than one gift
    int result = 0;
    for (auto& house : houses) {
        if (house.get_gifts() >= 1)
            ++result;
    }

    std::cout << result << std::endl;

    return 0;
}
