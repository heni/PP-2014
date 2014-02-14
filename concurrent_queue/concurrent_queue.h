#include <boost/thread.hpp>
#include <queue>

template<class T>
class TConcurrentQueue {
    std::queue<T> InternalQueue;
    boost::mutex Mutex;
    bool ClosingFlag;

    boost::condition_variable Condition;
public:
    TConcurrentQueue()
        : ClosingFlag(false)
    {}

    void Put(const T& t) {
        boost::lock_guard<boost::mutex> lk(Mutex);
        InternalQueue.push(t);
        Condition.notify_one();
    }

    bool Get(T& t) {
        boost::unique_lock<boost::mutex> lk(Mutex);

        Condition.wait(lk, [this]()->bool {
            return !InternalQueue.empty() || ClosingFlag;
        });
        if (!InternalQueue.empty()) {
            t = InternalQueue.front();
            InternalQueue.pop();
            return true;
        }
        return false;
    }
    
    void SetClosingFlag() {
        boost::lock_guard<boost::mutex> lk(Mutex);
        ClosingFlag = true;
        Condition.notify_all();
    }
};
