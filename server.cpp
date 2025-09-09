#include <iostream>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <cstring>
#include <unistd.h>
#include <sys/epoll.h>
#include <fcntl.h>
#include "util.h"

#define MAX_EVENTS      1024
#define READ_BUFFER     1024

void setnoblocking(int fd) {
    fcntl(fd, F_SETFL, fcntl(fd, F_GETFL) | O_NONBLOCK);
}

int main() {
    int ret = 0;
    int serverSocketFd = socket(AF_INET, SOCK_STREAM, 0);
    errif(serverSocketFd == -1, "socket create error");

    struct sockaddr_in serverAddr;
    bzero(&serverAddr, sizeof(serverAddr));
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_addr.s_addr = inet_addr("127.0.0.1");
    serverAddr.sin_port = htons(8888);

    ret = bind(serverSocketFd, (sockaddr*)&serverAddr, sizeof(serverAddr));
    errif(ret == -1, "socket bind error");

    ret = listen(serverSocketFd, SOMAXCONN);
    errif(ret == -1, "socket listen error");

    int epollFd = epoll_create1(0);
    errif(epollFd == -1, "epoll create error");

    struct epoll_event events[MAX_EVENTS], ev;
    bzero(events, sizeof(events));

    bzero(&ev, sizeof(ev));
    ev.data.fd = serverSocketFd;
    ev.events = EPOLLIN | EPOLLET;
    setnoblocking(serverSocketFd);
    epoll_ctl(epollFd, EPOLL_CTL_ADD, serverSocketFd, &ev);
    
    while(true) {
        int nfds = epoll_wait(epollFd, events, MAX_EVENTS, -1);
        errif(nfds == -1, "epoll wait error");

        for(int i = 0; i < nfds; i++) {
            if(events[i].data.fd == serverSocketFd) {       // 新客户端建立连接
                struct sockaddr_in clientAddr;
                socklen_t clientAddrLen = sizeof(clientAddr);
                bzero(&clientAddr, sizeof(clientAddr));

                int clientSocketFd = accept(serverSocketFd, (sockaddr*)&clientAddr, &clientAddrLen);
                errif(clientSocketFd == -1, "socket accept error");

                std::cout << "New client connected! IP : " << inet_ntoa(clientAddr.sin_addr)
                        << " Port : " << ntohs(clientAddr.sin_port) << std::endl;
                
                bzero(&ev, sizeof(ev));
                ev.data.fd = clientSocketFd;
                ev.events = EPOLLIN | EPOLLET;
                setnoblocking(clientSocketFd);
                epoll_ctl(epollFd, EPOLL_CTL_ADD, clientSocketFd, &ev);
            } else if(events[i].events & EPOLLIN) {         // 可读事件
                char buf[READ_BUFFER];
                while(true) {
                    bzero(buf, sizeof(buf));
                    ssize_t readBytes = read(events[i].data.fd, buf, sizeof(buf));
                    if(readBytes > 0) {
                        std::cout << "message from client, fd  " << events[i].data.fd << " : " << buf << std::endl;
                        write(events[i].data.fd, buf, sizeof(buf));
                    } else if(readBytes == -1 && errno == EINTR) {     // 客户端正常中断, 继续读取
                        std::cout << "continue reading " << std::endl;
                        continue;
                    } else if (readBytes == -1 && ((errno == EAGAIN) || (errno == EWOULDBLOCK))) { //非阻塞IO, 这个条件表示数据全部读取完毕
                        std::cout << "finish reading once. erron : " << errno << std::endl;
                        break;
                    } else if(readBytes == 0) {     // EOF, 客户端断开连接
                        std::cout << "EOF, client fd : " << events[i].data.fd << " disconnected" << std::endl;
                        close(events[i].data.fd);
                        break;
                    }
                }
            } else {        // 其他事件, 待实现
                std::cout << "something else happened." << std::endl;
            }
        }
    }

    close(serverSocketFd);

    return 0;
}