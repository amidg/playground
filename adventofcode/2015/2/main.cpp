#include <iostream>
#include <fstream>
#include <vector>
#include "gift.h"

std::vector<Gift> gifts;
int total_area = 0;
int total_ribbon = 0;

int main(int argc, char* argv[]) {
    std::ifstream file("input.txt");
    std::string line; 
    while (std::getline(file, line)) {
        gifts.push_back(Gift(line));
    }

    for (auto& gift : gifts) {
        // get total area
        total_area += gift.get_gift_area();

        // get total ribbon length
        total_ribbon += gift.get_ribbon_length();

    }
    std::cout << "Total Area: " << total_area << std::endl;
    std::cout << "Total Ribbon: " << total_ribbon << std::endl;

    return 0;
}
