#include <iostream>
#include <thread>
#include <mutex>
#include <condition_variable>

class PingPong {
private:
    // main
    int capacity;

    // threading
    std::mutex mtx;
    std::condition_variable cv;

    // protected data
    std::string* buffer;
    bool isDone = false;
    bool isPing = true;
    bool isPingDone = false;
    bool isPongDone = false;
    int size = 0;
public:
    // constructors
    PingPong(int cap) {
        capacity = cap;
        buffer = new std::string[capacity];
    };
    ~PingPong() { delete[] buffer; };

    // thread functions
    void ping() {
        bool* isPingLocal = &isPing;
        for (int i = 0; i < 10; i++) {
            std::unique_lock<std::mutex> lock(mtx);
            cv.wait(lock, [=]{ return *isPingLocal; });
            buffer[size++] = "ping";
            isPing = false;
            //std::cout << buffer[size-1] << std::endl;
            cv.notify_all();
        }
        isPingDone = true;
    }

    void pong() {
        bool* isPingLocal = &isPing;
        for (int i = 0; i < 10; i++) {
            std::unique_lock<std::mutex> lock(mtx);
            cv.wait(lock, [=]{ return !(*isPingLocal); });
            buffer[size++] = "pong";
            isPing = true;
            //std::cout << buffer[size-1] << std::endl;
            cv.notify_all();
        }
        isPongDone = true;
    }

    void print() {
        bool* isPingDoneLocal = &isPingDone;
        bool* isPongDoneLocal = &isPongDone;
        int index = 0;
        while (!isDone || index < size) {
            std::unique_lock<std::mutex> lock(mtx);
            cv.wait(lock, [=]{ return *isPingDoneLocal && *isPongDoneLocal;});
            if (index >= size) break;
            std::cout << index << ": " << buffer[index++] << std::endl;
            cv.notify_all();
        }
    }

    // other
    void set_done() {
        std::unique_lock<std::mutex> lock(mtx);
        isDone = true;
    }
};

int main(int argc, char* argv[]) {
    PingPong pp(40);
    std::thread t1(&PingPong::ping, &pp);
    std::thread t3(&PingPong::pong, &pp);
    std::thread t2(&PingPong::print, &pp);

    t1.join();
    t2.join();
    pp.set_done();
    t3.join();

    std::cout << "Done" << std::endl;

    return 0;
}
