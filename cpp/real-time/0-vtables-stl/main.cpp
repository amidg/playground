#include <iostream>
#include <chrono>
#include <ctime>

// Q1
// EXPLANATION:
// vTable Base ---> vTable Derived
// slot 1: get_count  ---> get_count (direct copy)
// slot 2: count --------> count (overriden)
// slot 3: ~Base() ------> ~Derived (overriden)
// slot 4:                 get_count_new (new virtual function)
// vPtr is the pointer to the vTable, table of the virtual functions. This table is used to store all the virtual functions' addresses
// vPtr is handled by the compiler. If any virtual/inherited functions are present:
// - The compiler reserves space for the vptr at the beginning of the object's memory layout and emits code in the constructor to initialize it at runtime
// The derived class does not overwrite or physically alter the base class's vTable; instead, it copies the structure of the base class's vTable and modifies its contents based on whether functions are overridden
// If multiple classes are inherited, new class contains multiple vPtr
class Base {
public:
    Base() {cnt_ = 0;};

    virtual void get_count() {
        printf("[Base] new count is %ld\n", cnt_);
    };

    virtual void count() {
        ++cnt_;
    };

    void hello_world() {
        std::cout << "[Base] hello world" << std::endl;
    };

    // always make base classes' destructors virtual when they're meant to be manipulated polymorphically.
    // deleting through the base class pointer means that static type (Derived) is manipulated through the dynamic type (Base)
    // This has undefined behavior
    virtual ~Base() { std::cout << "Base destructor is called" << std::endl; };
protected:
    size_t cnt_;
};

class Derived : public Base {
public:
    void count() override {
        cnt_ += 2;
    }

    virtual void get_count_new() {
        printf("[Derived] new count is %ld\n", cnt_);
    };

    ~Derived() { std::cout << "Derived destructor" << std::endl; };
};

// Q2
// EXPLANATION:
// Using clock_gettime means using POSIX time function exposed via <ctime> header. Cannot use std::function<> because it introduces its own overhead thus affecting time measurement
// - Virtual call stack if slower: 
// Base time: 7.508849 (ms)/ Derived time: 0.000387 (ms)
// - Virtual functions are slower because they cannot be optimized by compiler and thus require runtime lookup for function's own address
// - If vTable is currently not loaded in the cache, this causes cache miss
// - Virtual functions cannot be inlined at compile time because their exact intention is known only at the runtime
// - Compiler can devirtualize only if exact virtual function is known at compile time: keyword final or calling object directly
// - O2 optimizations already include the -fdevirtualize flag 
// - Looping back to virtual functions, if you've got a virtual function you need to use new and can't use malloc to put one on the heap. This is because new will set up the vtable, which malloc won't, and once it's not setup, you can't initialise it any more.
// - Virtual functions can, occasionally, create performance issues. The main ones being:
// 1. it becomes much harder to inline functions (a very common and effective optimisation - can be helped with a judicious use of final);
// 2. it can bloat the size of the class (imagine if the actual data inside is just a 32-bit integer. Your 64-bit pointer to hold the vtable could force you to accept 32-bits of padding as well, and end up doubling the size of the class in memory, meaning you get half as many in a cache line);
// 3. pointer dereference every time you call a virtual function, which can lead to a cache miss, which can also hurt performance.


// Q3
// EXPLANATION:
// - 3 binary instances of this template square() function
// - They live inside the .o and eventually inside the binary
// - Translation Units aka final source fed to the compiler (.cpp) where ODR is applicable
// ODR (One Definition Rule) has unique relationship with the template fucntions because of the following reasons:
// - All template functions are considered token-to-token identical and implicitely inlined
// - Template functions can live inside the multiple Translation Units as long as they are all identical
template<typename T>
T square(T x) { return x*x; }

// Q4
// EXPLANATION:
// - Compiles for print_size(hello_world)
// - Does not compile for the print_size(q4)
// g++ -std=c++20 -Wall -Wextra -O2 -o problem_0 main.cpp
// main.cpp: In instantiation of ‘void print_size(T) [with T = int]’:
// main.cpp:115:15:   required from here
//   115 |     print_size(q4);
//       |     ~~~~~~~~~~^~~~
// main.cpp:79:19: error: static assertion failed: Object of this type does not have size() method
//    79 |     static_assert(HasSize<T>, "Object of this type does not have size() method");
//       |                   ^~~~~~~~~~
//   • constraints not satisfied
//     • required by the constraints of ‘template<class T> concept HasSize’
//       main.cpp:73:9:   
//          73 | concept HasSize = requires (T x) {
//             |         ^~~~~~~
//     • in requirements with ‘T x’ [with T = int]
//       main.cpp:73:19:   
//          73 | concept HasSize = requires (T x) {
//             |                   ^~~~~~~~~~~~~~~~
//          74 |     { x.size() };
//             |     ~~~~~~~~~~~~~  
//          75 | };
//             | ~                  
//     • the required expression ‘x.size()’ is invalid
//       main.cpp:74:13:
//          74 |     { x.size() };
//             |       ~~~~~~^~
//     • set ‘-fconcepts-diagnostics-depth=’ to at least 2 for more detail
// main.cpp:80:20: error: request for member ‘size’ in ‘x’, which is of non-class type ‘int’
//    80 |     std::cout << x.size() << std::endl;
//       |
template<typename T>
concept HasSize = requires (T x) {
    { x.size() };
};

template<typename T>
void print_size(T x) {
    static_assert(HasSize<T>, "Object of this type does not have size() method");
    std::cout << x.size() << std::endl;
}

// Q5
// EXPLANATION:
// override has never been a requirement in C++, as compiler will try to guess my intentions. override keyword allows to clearly indicate the software intention both to compiler and other developers. This is key in the robotics software to make it maintainable, readable and safe. Example below:
// - FloatTick compiles with warning "hidden method" because it DOES NOT overwrite the method, instead it creates the new one with the float signature.
// - BadDerivedTick will try to override as it follows standard design pattern using override to indicate the intention. This fails because signature pattern is different
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Woverloaded-virtual"
class BaseTick {
public:
    virtual void tick(int ms) { std::cout << ms << std::endl; };
};

// this produces warning due to hidden method
class FloatTick : public BaseTick {
public:
    void tick(float ms) { std::cout << ms + 2.0 << std::endl; };
};
#pragma GCC diagnostic pop

#if false
class BadDerivedTick : public BaseTick {
public:
    void tick(float ms) override { std::cout << ms + 3.0 << std::endl; };
};
#endif

int main(/*int argc, char * argv[]*/) {
    // -------
    // q1
    Base base;
    base.count();
    base.get_count();
    base.hello_world();

    Derived derived;
    derived.count(); // derived gets its own instance of the cnt_
                      // does not share instance with base
    derived.get_count_new();

    // -------
    // q2
    Base* base_ptr = new Derived();
    double base_func_time, derived_func_time;
    long seconds, nanoseconds;
    struct timespec start, end;

    // time calculation based on the base pointer
    if (clock_gettime(CLOCK_MONOTONIC, &start) != 0) {
        std::cerr << "Clock read failed!" << std::endl;
        return 1;
    }

    for (size_t i = 0; i < 10'000'000; i++) {
        base_ptr->count();
    }
    clock_gettime(CLOCK_MONOTONIC, &end);
    seconds = end.tv_sec - start.tv_sec;
    nanoseconds = end.tv_nsec - start.tv_nsec;
    base_func_time = (seconds * 1000.0) + (nanoseconds / 1000000.0);

    // time calculation for the derived pointer
    if (clock_gettime(CLOCK_MONOTONIC, &start) != 0) {
        std::cerr << "Clock read failed!" << std::endl;
        return 1;
    }

    for (size_t i = 0; i < 10'000'000; i++) {
        derived.count(); // use stack allocated to avoid measuring noise
    }
    clock_gettime(CLOCK_MONOTONIC, &end);
    seconds = end.tv_sec - start.tv_sec;
    nanoseconds = end.tv_nsec - start.tv_nsec;
    derived_func_time = (seconds * 1000.0) + (nanoseconds / 1000000.0);

    printf("Base time: %f (ms)/ Derived time: %f (ms)\n", base_func_time, derived_func_time);
    delete base_ptr;

    // -------
    // q3
    int x = 2;
    double xd = 2.0;
    float xf = 2.0;
    int fi = square(x);
    double fd = square(xd);
    float ff = square(xf);
    printf("Q3: integer %d / double %f / float %f\n", fi, fd, ff);

    // q4
    std::string hello_world = "Hello World";
    print_size(hello_world); // good, because std::string has size()
    // int q4 = 10;
    // print_size(q4); -> will not compile

    // q5
    BaseTick tick;
    FloatTick ftick;
    tick.tick(1); // calls base class
    ftick.tick(2.0); // calls base class
    // BadDerivedTick produces error due to signature mismatch
    // BadDerivedTick bad_tick;
    // bad_tick.tick(2.0);

    return 0;
}

