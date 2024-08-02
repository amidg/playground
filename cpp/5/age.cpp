#include <iostream>
#include <string>
#include <vector>
#include <algorithm>

typedef struct Person {
    std::string name;
    int age;
} Person;

std::vector<Person> people;

bool get_person_info() {
    std::string name;
    int age;
    std::cout << "Enter person name: ";
    std::getline(std::cin, name);
    if (name.empty())
        return 0; // if just enter then break
    std::cout << "Enter " << name << "'s age: ";
    std::cin >> age;
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    Person person = {name, age};
    people.push_back(person);
    return 1;
}

int oldest_person_index(const std::vector<Person>& v) {
    int old_i = 0;
    for (int i = 0; i < v.size(); i++)
        if (v[old_i].age < v[i].age)
            old_i = i;
    return old_i;
}

int main(int argc, char* argv[]) {
    // collect people info
    while (get_person_info());
    std::cout << "Done collecting people data" << std::endl;

    // get oldest person
    int i = oldest_person_index(people);
    std::cout << "Oldest person is " << people[i].name << " " << people[i].age <<" years old" << std::endl;
    return 0;
}
