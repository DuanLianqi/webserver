#include <functional>
#include "Server.h"
#include "Socket.h"
#include "Acceptor.h"
#include "Connection.h"

Server::Server(EventLoop *loop) : loop(loop), acceptor(nullptr) {
    acceptor = new Acceptor(loop);
    std::function<void(Socket*)> callback = std::bind(&Server::newConnection, this, std::placeholders::_1);
    acceptor->setNewConnectionCallback(callback);
}

Server::~Server() {
    delete acceptor;
}

void Server::newConnection(Socket *serverSocket) {
    Connection *connection = new Connection(loop, serverSocket);
    std::function<void(Socket*)> callback = std::bind(&Server::deleteConnection, this, std::placeholders::_1);
    connection->setDeleteConnectionCallback(callback);
    connections[serverSocket->getFd()] = connection;
}

void Server::deleteConnection(Socket *serverSocket) {
    Connection *connection = connections[serverSocket->getFd()];
    connections.erase(serverSocket->getFd());
    delete connection;
}
