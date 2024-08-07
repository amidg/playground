#include <iostream>
#include <thread>
#include <mutex>
#include <condition_variable>

class PingPong {
private:
    std::mutex mtx;
    std::condition_variable cv;

    // protected data
    std::string* buffer;
    bool isDone = false;
    bool 
};

int main(int argc, char* argv[]) {
    PingPong pp(40);
    std::thread t1(&pp.ping, &pp);

    return 0;
}
