#include <string>
#include <iostream>
#include <vector>
#include <cassert>
//#include "player.h"

namespace Items {
    enum Type { // unscoped enum
        potion, // 0
        torch, // 1
        arrow, // 2
        max_items // 3
    };
};

void count_total_items(const std::vector<int>& inventory) {
    int cnt = 0;
    Items::Type type;
    for (int i = 0; i < inventory.size(); i++) {
        cnt += inventory[i]; // calculate total items anyway
        switch (i) {
            case Items::potion:
                std::cout << "You have " << inventory[i] << ((inventory[i] == 1) ? " potion" : " potions") << std::endl;
                break;
            case Items::torch:
                std::cout << "You have " << inventory[i] << ((inventory[i] == 1) ? " torch" : " torches") << std::endl;
                break;
            case Items::arrow:
                std::cout << "You have " << inventory[i] << ((inventory[i] == 1) ? " arrow" : " arrows") << std::endl;
                break;
        }

    }
    std::cout << "You have " << cnt << " total items" << std::endl;
}

int main(int argc, char* argv[]) {
    std::vector<int> inventory {1, 5, 10};
    assert(inventory.size() == Items::max_items);
    count_total_items(inventory);
    return 0;
}
