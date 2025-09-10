#include <iostream>
#include <cstring>
#include <unistd.h>
#include "InetAddress.h"
#include "Socket.h"
#include "util.h"

int main() {
    Socket *clientSocket = new Socket();
    InetAddress *servAddr = new InetAddress("127.0.0.1", 8888);

    clientSocket->connect(servAddr);

    while(true) {
        char buf[1024];
        bzero(buf, sizeof(buf));
        std::cin >> buf;
        ssize_t writeBytes = write(clientSocket->getFd(), buf, sizeof(buf));

        if(writeBytes == -1) {
            std::cout << "socket already disconnected, can't write any more" << std::endl;
            break;
        }

        bzero(buf, sizeof(buf));
        ssize_t readBytes = read(clientSocket->getFd(), buf, sizeof(buf));
        if(readBytes > 0) {
            std::cout << "message from server : " << buf << std::endl;
        } else if(readBytes == 0) {
            std::cout << "server socket disconnected" << std::endl;
            break;
        } else if(readBytes == -1) {
            errif(true, "socket read error");
            break;
        }
    }

    delete clientSocket;
    delete servAddr;
    
    return 0;
}