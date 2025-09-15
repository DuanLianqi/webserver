#include <iostream>
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

    delete address;
}

Acceptor::~Acceptor() {
    delete socket;
    delete acceptChannel;
}

void Acceptor::acceptConnection() {
    InetAddress *clientAddress = new InetAddress();
    int clientSocketFd = socket->accept(clientAddress);
    Socket *clientSocket = new Socket(clientSocketFd);

    std::cout << "New client connected! IP : " << inet_ntoa(clientAddress->addr.sin_addr)
            << " Port : " << ntohs(clientAddress->addr.sin_port) << std::endl;

    clientSocket->setnonblocking();
    newConnectionCallback(clientSocket);

    delete clientAddress;
}

void Acceptor::setNewConnectionCallback(std::function<void(Socket *)> callback) {
    newConnectionCallback = callback;
}
