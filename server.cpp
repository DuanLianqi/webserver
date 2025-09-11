#include <iostream>
#include <cstring>
#include <unistd.h>
#include "Socket.h"
#include "InetAddress.h"
#include "Epoll.h"
#include "Channel.h"
#include "util.h"

#define MAX_EVENTS      1024
#define READ_BUFFER     1024

void handleReadEvent(int sockfd) {
    char buf[READ_BUFFER];
    while(true) {
        bzero(buf, sizeof(buf));
        ssize_t readBytes = read(sockfd, buf, sizeof(buf));
        if(readBytes > 0) {
            std::cout << "message from client, fd  " << sockfd << " : " << buf << std::endl;
            write(sockfd, buf, sizeof(buf));
        } else if(readBytes == -1 && errno == EINTR) {     // 客户端正常中断, 继续读取
            std::cout << "continue reading " << std::endl;
            continue;
        } else if (readBytes == -1 && ((errno == EAGAIN) || (errno == EWOULDBLOCK))) { //非阻塞IO, 这个条件表示数据全部读取完毕
            std::cout << "finish reading once. erron : " << errno << std::endl;
            break;
        } else if(readBytes == 0) {     // EOF, 客户端断开连接
            std::cout << "EOF, client fd : " <<sockfd << " disconnected" << std::endl;
            close(sockfd);
            break;
        }
    }
}

int main() {
    Socket *serverSocket = new Socket();
    InetAddress *servAddr = new InetAddress("127.0.0.1", 18888);
    Epoll *ep = new Epoll();

    serverSocket->bind(servAddr);
    serverSocket->listen();
    serverSocket->setnonblocking();

    Channel *serverChannel = new Channel(ep, serverSocket->getFd());
    serverChannel->enableReading();

    while(true) {
        std::vector<Channel*> activeChannel = ep->poll();
        int nfds = activeChannel.size();

        for(int i = 0; i < nfds; i++) {
            if(activeChannel[i]->getFd() == serverSocket->getFd()) {         //新客户端连接
                InetAddress *clientAddr = new InetAddress();
                int clientSocketFd = serverSocket->accept(clientAddr);
                Socket *clientSocket = new Socket(clientSocketFd);

                std::cout << "New client connected! IP : " << inet_ntoa(clientAddr->addr.sin_addr)
                        << " Port : " << ntohs(clientAddr->addr.sin_port) << std::endl;

                clientSocket->setnonblocking();
                Channel *clientChannel = new Channel(ep, clientSocketFd);
                clientChannel->enableReading();
            } else if(activeChannel[i]->getRevents() & EPOLLIN) {         // 可读事件
                handleReadEvent(activeChannel[i]->getFd());
            } else {        // 其他事件, 待实现
                std::cout << "something else happened." << std::endl;
            }
        }
    }

    delete serverSocket;
    delete servAddr;

    return 0;
}