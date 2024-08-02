#include <iostream>
#include <vector>
#include <algorithm>
#include <numeric>

int accumulate(int num) {
    static std::vector<int> numbers;
    numbers.push_back(num);
    return std::accumulate(numbers.begin(), numbers.end(), 0);
}

int main()
{
    std::cout << accumulate(4) << '\n'; // prints 4
    std::cout << accumulate(3) << '\n'; // prints 7
    std::cout << accumulate(2) << '\n'; // prints 9
    std::cout << accumulate(1) << '\n'; // prints 10

    return 0;
}
