#include <iostream>

class Test {
	public:
		Test(): id(++var) {}
		int get_value() const { return id; };
		static int var;
	private:
		int id;
};
int Test::var = 10;

int main() {
	Test* arr[10];
	for (int i = 0; i < 10; i++)
		arr[i] = new Test;
	for (int i = 0; i < 10; i++)
		std::cout << arr[i]->get_value() << std::endl;
	return 0;
}
