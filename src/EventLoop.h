#ifndef EVENTLOOP_H
#define EVENTLOOP_H

#include <functional>

class Epoll;
class Channel;
class ThreadPool;
class EventLoop {
public:
    EventLoop();
    ~EventLoop();

    void loop();
    void updateChannel(Channel *channel);

    void addThreadTask(std::function<void()> task);

private:
    ThreadPool *threadpool;
    Epoll *ep;
    bool quit;
};

#endif
