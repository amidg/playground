#include <iostream>
#include <string>

double val1 = 0;
double val2 = 0;
char sign = ' ';

template<typename T>
void recordEntry(std::string text, T& var) {
	std::cout << text;
	std::cin >> var;
    std::cout << std::endl;
}


int main(int argc, char* argv[]) {
	// get operation
	recordEntry<double>("Enter value 1: ", val1);
	recordEntry<double>("Enter value 2: ", val2);
	recordEntry<char>("Enter Operation +, -, *, /: ", sign);

	// calculate the result
	switch (sign) {
		case '+':
            val1 += val2;
            std::cout << "Result: " << val1 <<std::endl;
			break;
        case '-':
            val1 -= val2;
            std::cout << "Result: " << val1 <<std::endl;
            break;
        case '*':
            val1 *= val2;
            std::cout << "Result: " << val1 <<std::endl;
            break;
        case '/':
            if (val2 != 0) {
                val1 /= val2;
                std::cout << "Result: " << val1 <<std::endl;
            } else
                std::cout << "Cannot divide by 0" << std::endl;
            break;
        default:
            std::cout << "Unsupported operation: " << sign << std::endl;
            break;
	}

	return 0;
}
