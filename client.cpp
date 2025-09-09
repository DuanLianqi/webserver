#include <iostream>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <cstring>
#include <unistd.h>
#include "util.h"

int main() {
    int ret = 0;
    int clientSocketFd = socket(AF_INET, SOCK_STREAM, 0);
    errif(clientSocketFd == -1, "create socket error");

    struct sockaddr_in serverAddr;
    bzero(&serverAddr, sizeof(serverAddr));
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_addr.s_addr = inet_addr("127.0.0.1");
    serverAddr.sin_port = htons(8888);

    ret = connect(clientSocketFd, (sockaddr*)&serverAddr, sizeof(serverAddr));
    errif(ret == -1, "socket connect error");

    while(true) {
        char buf[1024];
        bzero(buf, sizeof(buf));
        std::cin >> buf;
        ssize_t writeBytes = write(clientSocketFd, buf, sizeof(buf));

        if(writeBytes == -1) {
            std::cout << "socket already disconnected, can't write any more" << std::endl;
            break;
        }

        bzero(buf, sizeof(buf));
        ssize_t readBytes = read(clientSocketFd, buf, sizeof(buf));
        if(readBytes > 0) {
            std::cout << "message from server : " << buf << std::endl;
        } else if(readBytes == 0) {
            std::cout << "server socket disconnected" << std::endl;
            break;
        } else if(readBytes == -1) {
            close(clientSocketFd);
            errif(true, "socket read error");
        }
    }

    close(clientSocketFd);
    
    return 0;
}