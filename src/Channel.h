#ifndef CHANNEL_H
#define CHANNEL_H

#include <cstdint>
#include <functional>

class Socket;
class EventLoop;

class Channel {
public:
    Channel(EventLoop *_loop, int _fd);
    ~Channel();

    int getFd();
    void handleEvent();
    void enableRead();
    uint32_t getEvents();
    uint32_t getReady();
    bool getInEpoll();
    void setInEpoll(bool _inEpoll = true);
    void useET();
    void setReady(uint32_t _ready);
    void setReadCallBack(std::function<void()>);
    void setUseThreadPool(bool use = true);

private:
    EventLoop *loop;
    int fd;
    uint32_t events;
    uint32_t ready;
    bool inEpoll;
    bool useThreadPool;
    std::function<void()> readCallback;
    std::function<void()> writeCallback;
};

#endif
