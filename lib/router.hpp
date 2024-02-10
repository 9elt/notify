#include "consts.cpp"
#include "notification.hpp"
#include "request.hpp"
#include <netinet/in.h>

char *response(string headers, string body);

string handle_req(sockaddr_in addr, Request request);
