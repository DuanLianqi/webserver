#ifndef SERVER_H
#define SERVER_H

class EventLoop;
class Socket;
class Acceptor;

class Server {
public:
    Server(EventLoop*);
    ~Server();

    void hanleReadEvent(int socketfd);
    void newConnection(Socket *serverSocket);

private:
    EventLoop *loop;
    Acceptor *acceptor;
};

#endif
