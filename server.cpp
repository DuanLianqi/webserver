#include <iostream>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <cstring>
#include <unistd.h>
#include "util.h"

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

    struct sockaddr_in clientAddr;
    socklen_t clientAddrLen = sizeof(clientAddr);
    bzero(&clientAddr, sizeof(clientAddr));

    int clientSocketFd = accept(serverSocketFd, (sockaddr*)&clientAddr, &clientAddrLen);
    errif(clientSocketFd == -1, "socket accept error");

    std::cout << "New client connected! IP : " << inet_ntoa(clientAddr.sin_addr)
              << " Port : " << ntohs(clientAddr.sin_port) << std::endl;
    
    while(true) {
        char buf[1024];
        bzero(buf, sizeof(buf));
        ssize_t readBytes = read(clientSocketFd, buf, sizeof(buf));

        if(readBytes > 0) {
            std::cout << "message from client, fd  " << clientSocketFd << " : " << buf << std::endl;
            write(clientSocketFd, buf, sizeof(buf));
        } else if(readBytes == 0) {
            std::cout << "client fd " << clientSocketFd << " disconnected" << std::endl;
            close(clientSocketFd);
            break;
        } else if(readBytes == -1) {
            close(clientSocketFd);
            errif(true, "socket read error");
        }
    }

    close(serverSocketFd);

    return 0;
}