#include <iostream>
#include <cstring>
#include <unistd.h>
#include "Connection.h"
#include "Socket.h"
#include "Channel.h"

#define READ_BUFFER 1024

Connection::Connection(EventLoop *loop, Socket* socket) : loop(loop), socket(socket), channel(nullptr) {
    channel = new Channel(loop, socket->getFd());
    std::function<void()> callback = std::bind(&Connection::echo, this, socket->getFd());
    channel->setCallBack(callback);
    channel->enableReading();
}

Connection::~Connection() {
    delete channel;
    delete socket;
}

void Connection::echo(int sockfd) {
    char buf[READ_BUFFER];
    while(true) {   //由于使用非阻塞IO, 读取客户端buffer, 一次读取buf大小数据, 直到全部读取完毕
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
            deleteConnectionCallback(socket);
            break;
        }
    }
}

void Connection::setDeleteConnectionCallback(std::function<void(Socket *)> callback) {
    deleteConnectionCallback = callback;
}
