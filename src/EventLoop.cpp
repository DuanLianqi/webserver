#include <vector>
#include "EventLoop.h"
#include "Channel.h"
#include "Epoll.h"
#include "ThreadPool.h"

EventLoop::EventLoop() : ep(nullptr), quit(false) {
    ep = new Epoll();
    threadpool = new ThreadPool();
}

EventLoop::~EventLoop() {
    delete ep;
    delete threadpool;
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

void EventLoop::addThreadTask(std::function<void()> task) {
    threadpool->add(task);
}
