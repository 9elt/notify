#include "consts.cpp"
#include "notification.hpp"
#include "request.hpp"
#include <netinet/in.h>

struct Router {
    ClientsList clients;
    NotificationsList notifications;
    string handle_req(sockaddr_in addr, Request request);
};
