#ifndef SERVER_H
#define SERVER_H

#include <map>

class EventLoop;
class Socket;
class Acceptor;
class Connection;

class Server {
public:
    Server(EventLoop *_loop);
    ~Server();

    void newConnection(Socket *serverSocket);
    void deleteConnection(int sockfd);

private:
    EventLoop *loop;
    Acceptor *acceptor;
    std::map<int, Connection*> connections;
};

#endif
