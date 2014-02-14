#include <iostream>
#include "concurrent_queue.h"


void ThreadFunc(TConcurrentQueue<int>& queue) {
    int res;
    while (queue.Get(res)) {
        std::cout << res << std::endl;
    }
}


int main() {
    TConcurrentQueue<int> SampleQueue;
    boost::thread printer(boost::bind(ThreadFunc, boost::ref(SampleQueue)));
    
    SampleQueue.Put(3);
    SampleQueue.Put(2);
    SampleQueue.Put(15);
    SampleQueue.SetClosingFlag();

    printer.join();
    return 0;
};
