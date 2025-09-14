#include <vector>
#include "EventLoop.h"
#include "Channel.h"
#include "Epoll.h"

EventLoop::EventLoop() : ep(nullptr), quit(false) {
    ep = new Epoll();
}

EventLoop::~EventLoop() {
    delete ep;
}

void EventLoop::loop() {
    while(!quit) {
        std::vector<Channel*> channels;
        channels = ep->poll();
        for(auto ch : channels) {
            ch->handleEvent();
        }
    }
}

void EventLoop::updateChannel(Channel *channel) {
    ep->updateChannel(channel);
}
