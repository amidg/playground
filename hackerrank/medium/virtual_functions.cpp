#include <cmath>
#include <cstdio>
#include <vector>
#include <iostream>
#include <algorithm>
using namespace std;

class Person {
public:
    Person() {};
    explicit Person(std::string& name, int age): name(name), age(age) {};
    virtual void getdata()=0;
    virtual void putdata()=0;
protected:
    std::string name;
    int age;
};

class Professor: public Person {
public:
    Professor(): Person(), cur_id(++id) {};
    void getdata() override {
        std::string input;
        std::cin >> name >> age >> publications;
    }
    void putdata() override {
        std::cout << name << " " << age << " " << publications << " " << cur_id << std::endl;
    }
    static int id;
private:
    int publications;
    int cur_id;
};
int Professor::id = 0;

class Student: public Person {
public:
    Student(): Person(), cur_id(++id) { grades = new int[6]; };
    ~Student() { delete[] grades; }
    void getdata() override {
        int i = 0;
        std::string input;
        while (std::cin >> input) {
            switch (i) {
                case 0:
                    name = input;
                    break;
                case 1:
                    age = stoi(input);
                    break;
                case 2: case 3: case 4: case 5: case 6: case 7:
                    sum += stoi(input);
                    break;
            }
            if (++i > 7)
                break; // break entire loop to avoid locking
        }
    }
    void putdata() override {
        std::cout << name << " " << age << " " << sum << " " <<cur_id <<std::endl;
    }
    static int id;
private:
    int sum;
    int *grades;
    int cur_id;
};
int Student::id = 0;

int main(){

    int n, val;
    cin>>n; //The number of objects that is going to be created.
    Person *per[n];

    for(int i = 0;i < n;i++){

        cin>>val;
        if(val == 1){
            // If val is 1 current object is of type Professor
            per[i] = new Professor;

        }
        else per[i] = new Student; // Else the current object is of type Student

        per[i]->getdata(); // Get the data from the user.

    }

    for(int i=0;i<n;i++)
        per[i]->putdata(); // Print the required output for each object.

    return 0;

}

