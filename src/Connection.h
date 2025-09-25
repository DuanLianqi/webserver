#ifndef CONNECTION_H
#define CONNECTION_H

#include <functional>
#include <string>

class EventLoop;
class Socket;
class Channel;
class Buffer;

class Connection {
public:
    Connection(EventLoop *_loop, Socket* _socket);
    ~Connection();

    void echo(int sockfd);
    void send(int sockfd);
    void setDeleteConnectionCallback(std::function<void(int)> callback);

private:
    EventLoop *loop;
    Socket *socket;
    Channel *channel;
    std::function<void(int)> deleteConnectionCallback;
    std::string *inBuffer;
    Buffer *readBuffer;
};

#endif
