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
    void addThread(std::function<void()> task);

private:
    Epoll *ep;
    ThreadPool *threadpool;
    bool quit;
};

#endif
