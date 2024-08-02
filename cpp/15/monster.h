#ifndef MONSTER_H
#define MONSTER_H

#include <string>
#include <iostream>

class Monster {
    public:
        enum Type {
            dragon,
            goblin,
            ogre,
            orc,
            skeleton,
            troll,
            vampire,
            zombie
        };
        explicit Monster(Type type, std::string name, std::string roar, int hit):
            type(type), name(name), roar(roar), hit(hit) {};

        // getters
        int get_type() const {return type;};
        std::string get_name() const {return name;};
        std::string get_roar() const {return roar;};
        int get_hit() const {return hit;};
        void print() {
            switch (get_hit()) {
                case 0:
                    std::cout << "Monster " << get_name();
                    std::cout << " is dead" << std::endl;
                    break;
                default:
                    std::cout << "Monster " << get_name();
                    std::cout << " is " << get_type();
                    std::cout << " makes " << get_roar();
                    std::cout << " sound and has " << get_hit() << " hits" << std::endl;
                    break;
            }
        }
    private:
        Type type;
        std::string name;
        std::string roar;
        int hit;
};

#endif
