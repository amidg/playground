#include <iostream>
#include <array>

constexpr double gravity = 9.8;
const int simulation_time = 5;
std::array<double, simulation_time+1> ball_heights;

// gets tower height from user and returns it
double getTowerHeight() {
	std::cout << "Enter the height of the tower in meters: ";
	double towerHeight;
	std::cin >> towerHeight;
	return towerHeight;
}

// Returns ball height from ground after "seconds" seconds
constexpr double calculateBallHeight(int seconds) noexcept {
	// Using formula: [ s = u * t + (a * t^2) / 2 ], here u(initial velocity) = 0
	return (gravity * seconds*seconds) / 2.0;
}

// Prints ball height above ground
void printBallHeight(double ballHeight, int seconds) {
	if (ballHeight > 0.0)
		std::cout << "At " << seconds << " seconds, the ball is at height: " << ballHeight << " meters\n";
	else
		std::cout << "At " << seconds << " seconds, the ball is on the ground.\n";
}

// Calculates the current ball height and then prints it
// This is a helper function to make it easier to do this
void printCalculatedBallHeight(double towerHeight) {
    for (int i = 0; i <= simulation_time; i++) {
        ball_heights[i] = calculateBallHeight(i);
        printBallHeight(towerHeight - ball_heights[i], i);
    }
}

int main()
{
	const double towerHeight = getTowerHeight();
	printCalculatedBallHeight(towerHeight);
	return 0;
}
