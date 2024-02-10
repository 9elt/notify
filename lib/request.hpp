#include "types.hpp"

#define GET_NOTIFICATIONS 1
#define CREATE_NOTIFICATION 2
#define CREATE_CLIENT 3
#define DELETE_CLIENT 4
#define GET_VERSION 5

struct Request {
    int endpoint;
    string body;
    Request(string reqstr);
};
