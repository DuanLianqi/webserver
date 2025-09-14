#include "util.h"
#include <iostream>

void errif(bool condition, const char *errMsg) {
    if(condition) {
        perror(errMsg);
        exit(EXIT_FAILURE);
    }
}
