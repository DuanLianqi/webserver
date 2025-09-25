#ifndef SOCKET_H
#define SOCKET_H

class InetAddress;

class Socket {
public:
    Socket();
    Socket(int _fd);
    ~Socket();

    void bind(InetAddress* addr);
    void listen();
    void setnonblocking();
    int accept(InetAddress* addr);
    void connect(InetAddress* addr);
    int getFd();

private:
    int fd;
};

#endif
