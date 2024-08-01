#include <iostream>

#define GRAVITY_CONST 9.8 // m/s^2

double height = 0;
double current_position = 0;

int main(int argc, char* argv[]) {
    std::cout << "Enter height of the tower [m]: ";
    std::cin >> height;

    // calculate height
    for (int i = 0; height-current_position > 0; i++) {
        current_position = GRAVITY_CONST*pow(i, 2.0);
        std::cout << "Time [s]: " << i << " Current Distance From Ground [m]: " << ((height - current_position < 0) ? 0 : height-current_position) << std::endl;
    }

    return 0;
}
