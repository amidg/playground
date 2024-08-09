#include <string>
#include <iostream>
#include <vector>
#include <utility>
#include <algorithm>
#include <numeric>

enum Call {
    Min,
    Max
};

template<typename T>
std::pair<T, T> find_min_max(const std::vector<T>& v, Call type) { 
    std::pair<T,T> pair;
    switch (static_cast<int>(type)) {
        case Call::Min:
            pair.first = std::distance(v.begin(), std::min_element(v.begin(), v.end()));
            pair.second = v[pair.first];
            break;
        case Call::Max:
            pair.first = std::distance(v.begin(), std::max_element(v.begin(), v.end()));
            pair.second = v[pair.first];
            break;
    }
    return pair;
}

template<typename T>
void print_min_max(const std::vector<T> v) {
    // find minimum
    auto result = find_min_max<T>(v, Call::Min);
    std::cout << "Minimum:\n";
    std::cout << "Index: " << result.first << " Value: " << result.second << std::endl;
    result = find_min_max<T>(v, Call::Max);
    std::cout << "\nMaximum:\n";
    std::cout << "Index: " << result.first << " Value: " << result.second << std::endl;
    std::cout << std::endl;
}

int main(int argc, char* argv[]) {
    std::string input;
    std::vector<int> v;
    while(1) {
        std::getline(std::cin, input);
        if (input.empty())
            break;
        //std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // use this only after the std::cin >> var to avoid the extra newline characters
        v.push_back(stoi(input));
    }
    print_min_max(v);
    return 0;
}
