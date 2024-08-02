/*
 *  Welcome to Roscoe's potion emporium!
 *  Enter your name: Alex
 *  Hello, Alex, you have 85 gold.
 *  
 *  Here is our selection for today:
 *  0) healing costs 20
 *  1) mana costs 30
 *  2) speed costs 12
 *  3) invisibility costs 50
 *  Enter the number of the potion you'd like to buy, or 'q' to quit: a
 *  That is an invalid input.  Try again: 3
 *  You purchased a potion of invisibility.  You have 35 gold left.
 */
#include <string>
#include "game.h"

// create inventory
std::array<Potion, Potions::END_STORE> store = {
    Potion(Potions::Healing),
    Potion(Potions::Mana),
    Potion(Potions::Speed),
    Potion(Potions::Invisibility),
};

int main(int argc, char* argv[]) {
    Player alex {"Alex", 85}; // coc-clang gives error using C++11
    std::string input;
    // welcome to the store
    std::cout << "Welcome to Roscoe's potion emporium!\n";
    std::cout << "Enter your name: ";
    std::getline(std::cin, input);
    if (input.empty() || input != alex.get_name())
        return 1;
    std::cout << "Hello, " << input << ". You have " << alex.get_gold() << " gold\n";
    //std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    while(1) {
        std::cout << "Our inventory today: \n";
        for (int i = 0; i < Potions::END_STORE; i++) {
            auto potion = store[i].get_info();
            std::cout << std::get<0>(potion) << ") " << std::get<1>(potion) << " costs " << std::get<2>(potion) << std::endl;
        }
        std::cout << "Enter the number of the potion you'd like to buy, or 'q' to quit: ";
        std::getline(std::cin, input);
        int potion_num;
        while (1) {
            try {
                potion_num = stoi(input);
                if (potion_num >= 0 && potion_num < Potions::END_STORE) {
                    try {
                        alex.set_potion(static_cast<Potions::Type>(potion_num));
                    } catch (std::string msg) {
                        std::cout << msg << std::endl;
                    }
                    std::cout << "You purchased a potion of invisibility.  You have " << alex.get_gold() <<" gold left." << std::endl << std::endl;
                    break;
                }
            } catch (std::invalid_argument err) {
                if (input == "q") {
                    return 0;
                } else {
                    std::cout << "That is an invalid input.  Try again: ";
                    std::getline(std::cin, input);
                }
            }
        }
    }

    return 0;
}
