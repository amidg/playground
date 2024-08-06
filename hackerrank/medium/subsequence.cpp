#include <vector>
#include <iostream>

std::vector<int> find_subsequence(const std::vector<int>& arr) {
    std::vector<int> result;
    int cnt;
    for (auto i = arr.begin(); i != arr.end(); i++) {
        cnt = std::count(arr.begin(), arr.end(), *i);
        if (cnt > 1 && std::count(result.begin(), result.end(), *i) < cnt - 1) {
            result.push_back(*i);
        }
    }

    // check if ascending
    for (auto i = result.begin(); i < result.end()-1; i++) {
        if (*i > *(i+1))
            return std::vector<int>{-1};
    }
    return result;
}

int main() {
    std::vector<int> input = {2, 2, 3, 10, 3, 10, 6, 10};
    std::vector<int> output = find_subsequence(input);
    for (auto i = output.begin(); i != output.end(); i++) {
        std::cout << *i << std::endl;
    }

    return 0;
}
