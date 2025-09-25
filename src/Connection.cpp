#include <iostream>
#include <cstring>
#include <unistd.h>
#include "Connection.h"
#include "Socket.h"
#include "Channel.h"
#include "util.h"
#include "Buffer.h"

#define READ_BUFFER 1024

Connection::Connection(EventLoop *_loop, Socket* _socket) : loop(_loop), socket(_socket), channel(nullptr), inBuffer(new std::string()), readBuffer(nullptr) {
    channel = new Channel(loop, socket->getFd());
    channel->enableRead();
    channel->useET();
    std::function<void()> callback = std::bind(&Connection::echo, this, socket->getFd());
    channel->setReadCallBack(callback);
    channel->setUseThreadPool(true);

    readBuffer = new Buffer();
}

Connection::~Connection() {
    delete channel;
    delete socket;
    delete inBuffer;
    delete readBuffer;
}

void Connection::echo(int sockfd) {
    char buf[READ_BUFFER];
    while(true) {   //由于使用非阻塞IO, 读取客户端buffer, 一次读取buf大小数据, 直到全部读取完毕
        bzero(buf, sizeof(buf));
        ssize_t readBytes = read(sockfd, buf, sizeof(buf));
        if(readBytes > 0) {
            readBuffer->append(buf, readBytes);
        } else if(readBytes == -1 && errno == EINTR) {     // 客户端正常中断, 继续读取
            std::cout << "continue reading " << std::endl;
            continue;
        } else if (readBytes == -1 && ((errno == EAGAIN) || (errno == EWOULDBLOCK))) { //非阻塞IO, 这个条件表示数据全部读取完毕
            std::cout << "finish reading once. " << std::endl;
            std::cout << "message from client, fd : " << sockfd << ", context : " << readBuffer->str() << std::endl;
            send(sockfd);
            readBuffer->clear();
            break;
        } else if(readBytes == 0) {     // EOF, 客户端断开连接
            std::cout << "EOF, client fd : " <<sockfd << " disconnected" << std::endl;
            //deleteConnectionCallback(sockfd);
            break;
        } else {
            std::cout << "Connection rest by peer. errno : " << errno << std::endl;
            //deleteConnectionCallback(sockfd);
            break;
        }
    }
}

void Connection::send(int sockfd) {
    int dataSize = readBuffer->size();
    int dataBegin = dataSize;
    char buf[dataSize];
    strncpy(buf, readBuffer->str(), dataSize);

    while(dataBegin > 0) {
        ssize_t writeBytes = write(sockfd, buf + (dataSize - dataBegin), dataBegin);
        if(writeBytes == -1 && errno == EAGAIN) {
            std::cout << "send error, errno : " << errno << std::endl;
            break;
        }
        dataBegin -= writeBytes;
    }
}

void Connection::setDeleteConnectionCallback(std::function<void(int)> callback) {
    deleteConnectionCallback = callback;
}
