#include <sys/socket.h>
#include <unistd.h>
#include <fcntl.h>
#include "Socket.h"
#include "InetAddress.h"
#include "util.h"

Socket::Socket() : fd(-1) {
    fd = socket(AF_INET, SOCK_STREAM, 0);
    errif(fd == -1, "socket create error");
}

Socket::Socket(int _fd) : fd(_fd) {
    errif(fd == -1, "socket create error");
}

Socket::~Socket() {
    if(fd != -1) {
        close(fd);
        fd = -1;
    }
}

void Socket::bind(InetAddress *addr) {
    int ret = ::bind(fd, (sockaddr*)&(addr->addr), addr->addrLen);
    errif(ret == -1, "socket bind error");
}

void Socket::listen() {
    int ret = ::listen(fd, SOMAXCONN);
    errif(ret == -1, "socket listen error");
}

void Socket::setnonblocking() {
    fcntl(fd, F_SETFL, fcntl(fd, F_GETFL) | O_NONBLOCK);
}

int Socket::accept(InetAddress *addr) {
    int clientFd = ::accept(fd, (sockaddr*)&(addr->addr), &addr->addrLen);
    errif(clientFd == -1, "socket accept error");
    return clientFd;
}

void Socket::connect(InetAddress *addr) {
    int ret = ::connect(fd, (sockaddr*)&(addr->addr), addr->addrLen);
    errif(ret == -1, "socket connect error");
}

int Socket::getFd() {
    return fd;
}
