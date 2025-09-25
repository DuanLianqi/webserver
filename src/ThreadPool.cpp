#include "ThreadPool.h"
#include <stdexcept>

ThreadPool::ThreadPool(int size) : stop(false) {
    for(int i = 0; i < size; ++i) {
        threads.emplace_back(std::thread([this] {
            while(true) {
                std::function<void()> task;
                {
                    std::unique_lock<std::mutex> lock(tasksMutex);
                    condition.wait(lock, [this] {
                        return stop || !tasks.empty();
                    });
                    if(stop && tasks.empty()) {
                        return;
                    }
                    task = tasks.front();
                    tasks.pop();
                }
                task();
            }
        }));
    }
}

ThreadPool::~ThreadPool() {
    {
        std::unique_lock<std::mutex> lock(tasksMutex);
        stop = true;
    }
    condition.notify_all();
    for(std::thread &thread : threads) {
        if(thread.joinable()) {
            thread.join();
        }
    }
}
