#include <iostream>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <cstring>

int main() {
    int serverSocketFd = socket(AF_INET, SOCK_STREAM, 0);

    struct sockaddr_in serverAddr;
    bzero(&serverAddr, sizeof(serverAddr));
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_addr.s_addr = inet_addr("127.0.0.1");
    serverAddr.sin_port = htons(8888);

    bind(serverSocketFd, (sockaddr*)&serverAddr, sizeof(serverAddr));

    listen(serverSocketFd, SOMAXCONN);

    struct sockaddr_in clientAddr;
    socklen_t clientAddrLen = sizeof(clientAddr);
    bzero(&clientAddr, sizeof(clientAddr));

    int clientSocketFd = accept(serverSocketFd, (sockaddr*)&clientAddr, &clientAddrLen);

    std::cout << "New client connected! IP : " << inet_ntoa(clientAddr.sin_addr)
              << " Port : " << ntohs(clientAddr.sin_port) << std::endl;
    
    return 0;
}