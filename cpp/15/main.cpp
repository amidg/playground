#include "monster.h"

int main() {
	Monster skeleton{ Monster::skeleton, "Bones", "*rattle*", 4 };
    skeleton.print();

    Monster vampire{ Monster::vampire, "Nibblez", "*hiss*", 0 };
	vampire.print();
	return 0;
}
