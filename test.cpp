#include <iostream>
#include <cstring>
#include <unistd.h>
#include <functional>
#include "src/ThreadPool.h"
#include "src/util.h"
#include "src/Socket.h"
#include "src/InetAddress.h"
#include "src/Buffer.h"

void oneClient(int msgs, int wait) {
    Socket *sock = new Socket();
    InetAddress *addr = new InetAddress("127.0.0.1", 8888);
    sock->connect(addr);

    Buffer *sendBuffer = new Buffer();
    Buffer *readBuffer = new Buffer();

    sleep(wait);
    int count = 0;
    while(count < msgs) {
        sendBuffer->setBuf("Hello World! I am client.");
        ssize_t writeBytes = write(sock->getFd(), sendBuffer->str(), sendBuffer->size());

        if(writeBytes == -1) {
            std::cout << "socket already disconnected, can't write any more" << std::endl;
            break;
        }

        int alreayRead = 0;
        char buf[1024];
        while(true) {
            bzero(buf, sizeof(buf));
            ssize_t readBytes = read(sock->getFd(), buf, sizeof(buf));
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

    delete sock;
    delete addr;
    delete sendBuffer;
    delete readBuffer;
}

int main(int argc, char *argv[]) {
    int threads = 100;
    int msgs = 100;
    int wait = 0;

    int op;
    const char *str = "t:m:w:";
    while((op = getopt(argc, argv, str)) != -1) {
        switch(op) {
            case 't':
                threads = atoi(optarg);
                break;
            case 'm':
                msgs = atoi(optarg);
                break;
            case 'w':
                wait = atoi(optarg);
                break;
            default:
                std::cout << "Usage: " << argv[0] << " [-t threads] [-m msgs] [-w wait]" << std::endl;
                break;
        }
    }

    ThreadPool *pool = new ThreadPool();
    std::function<void()> task = std::bind(oneClient, msgs, wait);

    for(int i = 0; i < threads; i++) {
        pool->add(task);
    }
    delete pool;
    return 0;
}
