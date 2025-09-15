#ifndef CONNECTION_H
#define CONNECTION_H

#include <functional>

class EventLoop;
class Socket;
class Channel;

class Connection {
public:
    Connection(EventLoop *loop, Socket* socket);
    ~Connection();
    void echo(int sockfd);
    void setDeleteConnectionCallback(std::function<void(Socket*)> callback);
private:
    EventLoop *loop;
    Socket *socket;
    Channel *channel;
    std::function<void(Socket*)> deleteConnectionCallback;
};

#endif
