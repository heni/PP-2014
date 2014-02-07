#include <boost/thread.hpp>
#include <iostream>
#include <vector>
#include <memory>
#include <mutex>
using namespace std;

/*class TMutexGuard {
    boost::mutex& InnerMutex;
public:
    TMutexGuard(boost::mutex& mutex) : InnerMutex(mutex) {
        InnerMutex.lock();
    }

    ~TMutexGuard() {
        InnerMutex.unlock();
    }
};*/

class TThreadFunc {
    size_t Id;
    static boost::mutex Mutex;
public:
    TThreadFunc (size_t id) : Id(id) {}
    ~TThreadFunc() {}

    void operator()() const {
        std::lock_guard<boost::mutex> guard(Mutex);
        cout << "Hello, World from [" << Id << "]" << endl;
    }
};

boost::mutex TThreadFunc::Mutex;


int main() {
    vector<shared_ptr<boost::thread>> threads;
    size_t thCount = 10;
    size_t created = 0;
    try {
        for (created = 0; created < thCount; ++created) {
            threads.push_back(make_shared<boost::thread>(TThreadFunc(created)));
        }
    } catch (const std::exception& e) {
        cerr << "exception handled after " << created << " threads." << endl;
    }
    for (size_t i = 0; i < created; ++i) {
        threads[i]->join();
    }
    return 0;
}
