#include "src/Server.h"
#include "src/EventLoop.h"

int main() {
    EventLoop *loop = new EventLoop();
    Server *server = new Server(loop);
    loop->loop();

    delete server;
    delete loop;

    return 0;
}
