#ifndef ACCEPTOR_H
#define ACCEPTOR_H

#include <functional>

class EventLoop;
class Socket;
class Channel;

class Acceptor {
public:
    Acceptor(EventLoop *_loop);
    ~Acceptor();

    void acceptConnection();
    void setNewConnectionCallback(std::function<void(Socket*)>);

private:
    EventLoop *loop;
    Socket *socket;
    Channel *acceptChannel;
    std::function<void(Socket*)> newConnectionCallback;
};

#endif
