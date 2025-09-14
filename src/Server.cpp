#include <iostream>
#include <cstring>
#include <unistd.h>
#include <functional>
#include "Server.h"
#include "Socket.h"
#include "InetAddress.h"
#include "Channel.h"

#define READ_BUFFER    1024

Server::Server(EventLoop *loop) : loop(loop) {
    Socket *serverSocket = new Socket();
    InetAddress *serverAddr = new InetAddress("127.0.0.1", 8888);
    serverSocket->bind(serverAddr);
    serverSocket->listen();
    serverSocket->setnonblocking();

    Channel *serverChannel = new Channel(loop, serverSocket->getFd());
    std::function<void()> callback = std::bind(&Server::newConnection, this, serverSocket);
    serverChannel->setCallBack(callback);
    serverChannel->enableReading();
}

Server::~Server() {

}

void Server::hanleReadEvent(int socketfd) {
    char buf[READ_BUFFER];
    while(true) {   //由于使用非阻塞IO, 读取客户端buffer, 一次读取buf大小数据, 直到全部读取完毕
        bzero(buf, sizeof(buf));
        ssize_t readBytes = read(socketfd, buf, sizeof(buf));
        if(readBytes > 0) {
            std::cout << "message from client, fd  " << socketfd << " : " << buf << std::endl;
            write(socketfd, buf, sizeof(buf));
        } else if(readBytes == -1 && errno == EINTR) {     // 客户端正常中断, 继续读取
            std::cout << "continue reading " << std::endl;
            continue;
        } else if (readBytes == -1 && ((errno == EAGAIN) || (errno == EWOULDBLOCK))) { //非阻塞IO, 这个条件表示数据全部读取完毕
            std::cout << "finish reading once. erron : " << errno << std::endl;
            break;
        } else if(readBytes == 0) {     // EOF, 客户端断开连接
            std::cout << "EOF, client fd : " <<socketfd << " disconnected" << std::endl;
            close(socketfd);
            break;
        }
    }
}

void Server::newConnection(Socket *serverSocket) {
    InetAddress *clientAddr = new InetAddress();
    int clientSocketFd = serverSocket->accept(clientAddr);
    Socket *clientSocket = new Socket(clientSocketFd);

    std::cout << "New client connected! IP : " << inet_ntoa(clientAddr->addr.sin_addr)
            << " Port : " << ntohs(clientAddr->addr.sin_port) << std::endl;

    clientSocket->setnonblocking();
    Channel *clientChannel = new Channel(loop, clientSocketFd);
    std::function<void()> callback = std::bind(&Server::hanleReadEvent, this, clientSocketFd);
    clientChannel->setCallBack(callback);
    clientChannel->enableReading();
}
