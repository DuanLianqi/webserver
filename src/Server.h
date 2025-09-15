#ifndef SERVER_H
#define SERVER_H

#include <map>

class EventLoop;
class Socket;
class Acceptor;
class Connection;

class Server {
public:
    Server(EventLoop*);
    ~Server();

    void newConnection(Socket *serverSocket);
    void deleteConnection(Socket *serverSocket);

private:
    EventLoop *loop;
    Acceptor *acceptor;
    std::map<int, Connection*> connections;
};

#endif
