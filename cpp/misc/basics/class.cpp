#include <iostream>

class Box {
public:
    int length;
    int width;

    // Constructor with parameters
    Box(int length, int width) {
        this->length = length; // Assigns the value of parameter length to the class member length
        this->width = width;   // Assigns the value of parameter width to the class member width
    }

    // A member function that uses 'this' to return the object itself
    Box& resize(int newLength, int newWidth) {
        this->length = newLength;
        this->width = newWidth;
        return *this; // Returns the object itself so you can chain methods
    }

    void display() const {
        std::cout << "Length: " << length << ", Width: " << width << std::endl;
    }
};

int main() {
    Box box(10, 5);
    box.display(); // Initial dimensions

    // Chaining method calls
    box.resize(20, 10).resize(30, 15);
    box.display(); // Displays updated dimensions after chaining

    return 0;
}

