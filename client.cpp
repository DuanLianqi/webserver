#include <iostream>
#include <cstring>
#include <unistd.h>
#include "src/InetAddress.h"
#include "src/Socket.h"
#include "src/util.h"
#include "src/Buffer.h"

int main() {
    Socket *clientSocket = new Socket();
    InetAddress *servAddr = new InetAddress("127.0.0.1", 8888);

    Buffer *sendBuffer = new Buffer();
    Buffer *readBuffer = new Buffer();

    clientSocket->connect(servAddr);

    while(true) {
        sendBuffer->getline();
        ssize_t writeBytes = write(clientSocket->getFd(), sendBuffer->str(), sendBuffer->size());

        if(writeBytes == -1) {
            std::cout << "socket already disconnected, can't write any more" << std::endl;
            break;
        }

        int alreayRead = 0;
        char buf[1024];
        while(true) {
            bzero(buf, sizeof(buf));
            ssize_t readBytes = read(clientSocket->getFd(), buf, sizeof(buf));

            if(readBytes > 0) {
                readBuffer->append(buf, readBytes);
                alreayRead += readBytes;
            } else if(readBytes == 0) {
                std::cout << "server socket disconnected" << std::endl;
                exit(EXIT_SUCCESS);
            }

            if(alreayRead >= sendBuffer->size()) {
                std::cout << "message from server : " << readBuffer->str() << std::endl;
                break;
            }
        }

        readBuffer->clear();
    }

    delete clientSocket;
    delete servAddr;
    delete sendBuffer;
    delete readBuffer;

    return 0;
}
