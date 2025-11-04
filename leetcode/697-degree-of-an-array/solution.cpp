#include <iostream>
#include <vector>
#include <map>

class Solution {
public:
	Solution() {};

	// Destructor
    	//~MyClass() {
    	//    delete[] data;
    	//}

    	//// Copy Constructor
    	//MyClass(const MyClass& other) : data(nullptr) {
    	//    if (other.data != nullptr) {
    	//        data = new char[strlen(other.data) + 1];
    	//        strcpy(data, other.data);
    	//    }
    	//}

    	//// Move Constructor
    	//MyClass(MyClass&& other) noexcept : data(other.data) {
    	//    other.data = nullptr;
    	//}

    	//// Copy Assignment Operator
    	//MyClass& operator=(const MyClass& other) {
    	//    if (this != &other) {
    	//        delete[] data;

    	//        if (other.data != nullptr) {
    	//            data = new char[strlen(other.data) + 1];
    	//            strcpy(data, other.data);
    	//        } else {
    	//            data = nullptr;
    	//        }
    	//    }
    	//    return *this;
    	//}

    	//// Move Assignment Operator
    	//MyClass& operator=(MyClass&& other) noexcept {
    	//    if (this != &other) {
    	//        delete[] data;
    	//        data = other.data;
    	//        other.data = nullptr;
    	//    }
    	//    return *this;
    	//}


	int findShortestSubArray(const std::vector& input) {
		if (input.empty())
			return 0;
		std::map<int, int> count = {};
		std::map<int, int> first_seen = {};
		std::map<int, int> last_seen = {};

		// populate entries
		for (int i = 0; i < input.size(); i++) {
			if (std::find(input.start(), input.end(), input[i]) == input.end())
				first_seen.insert()
				first_seen.append()
		}

	}

private:
	std::vector<int> nums;
}

int main() {
	std::vector<int> nums = {1,2,2,3,1,4,2};
	Solution solution = Solution();
	std::cout << solution.findShortestSubArray(nums) << std::endl;

	return 0;
}
