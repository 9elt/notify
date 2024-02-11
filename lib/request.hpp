#include "types.hpp"

#define GET_NOTIFICATIONS 1
#define CREATE_NOTIFICATION 2
#define GET_VERSION 3

struct Request {
    int endpoint;
    string body;
    Request(string reqstr);
};
