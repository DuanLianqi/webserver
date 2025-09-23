#include <iostream>
#include <string>
#include "src/ThreadPool.h"

void print(int a, double b, const char *c, std::string d) {
    std::cout << "a: " << a << ", b: " << b << ", c: " << c << ", d: " << d << std::endl;
}

void test() {
    std::cout << "Task executed in thread: " << std::this_thread::get_id() << std::endl;
}

int main(int argc, char *argv[]) {
    ThreadPool *pool = new ThreadPool();
    std::function<void()> task = std::bind(print, 42, 3.14, "Hello", "World");
    pool->add(task);
    task = test;
    pool->add(task);

    delete pool;
    return 0;
}
