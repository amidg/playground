#include <iostream>
#include <vector>

template <typename T>
void print_v(std::unique_ptr<std::vector<T>>& ptr) {
    for (auto i = ptr->begin(); i != ptr->end(); i++) {
        std::cout << *i;
    }
    std::cout << std::endl;
}

int main(int argc, char* argv[]) {
    std::unique_ptr<std::vector<int>> test1(new std::vector<int>);
    for (int i = 0; i < 5; i++) {
        test1->push_back(i);
    }
    print_v<int>(test1);
    
    std::unique_ptr<std::vector<int>> test2(std::move(test1));
    //print_v<int>(test1); // test1 is moved
    print_v<int>(test2);

    return 0;
}
