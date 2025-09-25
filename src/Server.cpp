#include <functional>
#include <unistd.h>
#include "Server.h"
#include "Socket.h"
#include "Acceptor.h"
#include "Connection.h"

Server::Server(EventLoop *_loop) : loop(_loop), acceptor(nullptr) {
    acceptor = new Acceptor(loop);
    std::function<void(Socket*)> callback = std::bind(&Server::newConnection, this, std::placeholders::_1);
    acceptor->setNewConnectionCallback(callback);
}

Server::~Server() {
    delete acceptor;
}

void Server::newConnection(Socket *serverSocket)
{
    if(serverSocket->getFd() != -1) {
        Connection *connection = new Connection(loop, serverSocket);
        std::function<void(int)> callback = std::bind(&Server::deleteConnection, this, std::placeholders::_1);
        connection->setDeleteConnectionCallback(callback);
        connections[serverSocket->getFd()] = connection;
    }

}

void Server::deleteConnection(int sockfd) {
    if(sockfd != -1) {
        auto it = connections.find(sockfd);
        if(it != connections.end()) {
            Connection *connection = connections[sockfd];
            connections.erase(sockfd);
            close(sockfd);
            //delete connection;
        }
    }
}
