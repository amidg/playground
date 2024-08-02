#ifndef GAME_H
#define GAME_H

#include <string>
#include <iostream>
#include <array>
#include <tuple>
#include <map>
#include <stdexcept>

namespace Potions {
    enum Type {         // unscoped enum
        Healing,        // 0
        Mana,           // 1
        Speed,          // 2
        Invisibility,   // 3
        END_STORE
    };

    std::map<Type, std::string> description {
        {Type::Healing, "healing"},
        {Type::Mana, "mana"},
        {Type::Speed, "speed"},
        {Type::Invisibility, "invisibility"}
    };

    std::map<Type, int> cost {
        {Type::Healing, 20},
        {Type::Mana, 30},
        {Type::Speed, 12},
        {Type::Invisibility, 50}
    };
};

class Potion {
    public:
        explicit Potion(Potions::Type type): type(type) {};

        std::tuple<Potions::Type, std::string, int> get_info() {
            return {type, Potions::description[type], Potions::cost[type]};
        };
        
    private:
        Potions::Type type;
};

class Player {
    public:
        explicit Player(std::string name, int money):
            name(name), gold(money) {};

        void set_potion(const Potions::Type type) {
            if (gold >= Potions::cost[type]) {
                inventory[type]++;
                gold -= Potions::cost[type];
            } else
                throw std::invalid_argument("Not enough gold");
        };

        // getters
        int get_gold() const {return gold;};
        std::string get_name() const {return name;};

    private:
        std::string name;
        std::array<int, Potions::END_STORE> inventory;
        int gold;
};

#endif
