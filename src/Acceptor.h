#ifndef ACCEPTOR_H
#define ACCEPTOR_H

#include <functional>

class EventLoop;
class Socket;
class InetAddress;
class Channel;

class Acceptor {
public:
    Acceptor(EventLoop *loop);
    ~Acceptor();
    void acceptConnection();
    std::function<void(Socket*)> newConnectionCallback;
    void setNewConnectionCallback(std::function<void(Socket*)>);
private:
    EventLoop *loop;
    Socket *socket;
    InetAddress *address;
    Channel *acceptChannel;
};

#endif
