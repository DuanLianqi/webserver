#ifndef THREADPOOL_H
#define THREADPOOL_H

#include <functional>
#include <vector>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <queue>

class ThreadPool {
public:
    ThreadPool(int size = 10);
    ~ThreadPool();

    void add(std::function<void()>);

private:
    std::vector<std::thread> threads;
    std::queue<std::function<void()>> tasks;
    std::mutex tasksMutex;
    std::condition_variable condition;
    bool stop;
};

#endif
