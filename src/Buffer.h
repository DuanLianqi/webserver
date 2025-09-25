#ifndef BUFFER_H
#define BUFFER_H

#include <string>

class Buffer {
public:
    Buffer();
    ~Buffer();

    void append(const char *str, int size);
    ssize_t size();
    const char* str();
    void clear();
    void getline();
    void setBuf(const char* str);

private:
    std::string buf;
};

#endif
