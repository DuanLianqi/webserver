#ifndef SERVER_H
#define SERVER_H

class EventLoop;
class Socket;

class Server {
public:
    Server(EventLoop*);
    ~Server();

    void hanleReadEvent(int socketfd);
    void newConnection(Socket *serverSocket);

private:
    EventLoop *loop;
};


#endif
