#include <unistd.h>
#include <cstring>
#include "Epoll.h"
#include "util.h"
#include "Channel.h"

#define MAX_EVENTS 1000

Epoll::Epoll() : epfd(-1), events(nullptr) {
    epfd = epoll_create1(0);
    errif(epfd == -1, "epoll create error");

    events = new epoll_event[MAX_EVENTS];
    bzero(events, sizeof(epoll_event) * MAX_EVENTS);
}

Epoll::~Epoll() {
    if(epfd != -1) {
        close(epfd);
        epfd = -1;
    }

    delete [] events;
}

void Epoll::addFd(int fd, uint32_t op) {
    struct epoll_event ev;
    bzero(&ev, sizeof(ev));
    ev.data.fd = fd;
    ev.events = op;

    int ret = epoll_ctl(epfd, EPOLL_CTL_ADD, fd, &ev);
    errif(ret == -1, "epoll add event error");
}

void Epoll::updateChannel(Channel *channel) {
    int ret = 0;
    int fd = channel->getFd();
    struct epoll_event ev;
    bzero(&ev, sizeof(ev));
    ev.data.ptr = channel;
    ev.events = channel->getEvents();

    if(channel->getInEpoll() == false) {
        ret = epoll_ctl(epfd, EPOLL_CTL_ADD, fd, &ev);
        errif(ret == -1, "epoll add error");
        channel->setInEpoll();
    } else {
        ret = epoll_ctl(epfd, EPOLL_CTL_MOD, fd, &ev);
        errif(ret == -1, "epoll modify error");
    }
}

std::vector<Channel*> Epoll::poll(int timeout) {
    std::vector<Channel*> activeChannel;
    int nfds = epoll_wait(epfd, events, MAX_EVENTS, timeout);
    errif(nfds == -1, "epoll wait error");
    for(int i = 0; i < nfds; i++) {
        Channel* channel = (Channel*)(events[i].data.ptr);
        channel->setRevents(events[i].events);
        activeChannel.push_back(channel);
    }
    return activeChannel;
}
