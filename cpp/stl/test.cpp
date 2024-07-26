#include <iostream>

template<typename T>
int compare(const T& num1, const T& num2) {
	return (num1 > num2) ? 1 : 0;
}

int main(int argc, char* argv[]) {
float num1 = 5.6;
float num2 = 6.4;
std::cout << compare(num1, num2) << std::endl;
return 1;
}
