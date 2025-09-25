#ifndef INETADDRESS_H
#define INETADDRESS_H

#include <arpa/inet.h>

class InetAddress {
public:
    InetAddress();
    InetAddress(const char *_ip, uint16_t _port);
    ~InetAddress();

    void setInetAddress(sockaddr_in addr);
    sockaddr_in getInetAddress();
private:
    struct sockaddr_in addr;
};

#endif
