#ifndef CHANNEL_H
#define CHANNEL_H

#include <sys/epoll.h>
#include <functional>

class EventLoop;

class Channel {
public:
    Channel(EventLoop *_loop, int _fd);
    ~Channel();

    void enableReading();

    int getFd();
    uint32_t getEvents();
    uint32_t getRevents();
    bool getInEpoll();
    void setInEpoll();

    void setRevents(uint32_t events);
    void setCallBack(std::function<void()>);

    void handleEvent();

private:
    EventLoop *loop;
    int fd;
    uint32_t events;
    uint32_t revents;
    bool inEpoll;
    std::function<void()> callback;
};

#endif
