#include "Acceptor.h"
#include "Socket.h"
#include "InetAddress.h"
#include "Channel.h"
#include "Server.h"

Acceptor::Acceptor(EventLoop *loop) : loop(loop) {
    socket = new Socket();
    address = new InetAddress("127.0.0.1", 8888);
    socket->bind(address);
    socket->listen();
    socket->setnonblocking();

    acceptChannel = new Channel(loop, socket->getFd());
    std::function<void()> callback = std::bind(&Acceptor::acceptConnection, this);
    acceptChannel->setCallBack(callback);
    acceptChannel->enableReading();
}

Acceptor::~Acceptor() {
    delete socket;
    delete address;
    delete acceptChannel;
}

void Acceptor::acceptConnection() {
    newConnectionCallback(socket);
}

void Acceptor::setNewConnectionCallback(std::function<void(Socket *)> callback) {
    newConnectionCallback = callback;
}
