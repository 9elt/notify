#include "consts.cpp"
#include "notification.cpp"
#include <arpa/inet.h>
#include <iostream>
#include <netinet/in.h>
#include <netinet/tcp.h>
#include <string>
#include <strings.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

using namespace std;

typedef unsigned int u32;

ClientsList clients;

NotificationsList notifications;

#define GET_NOTIFICATIONS 1
#define CREATE_NOTIFICATION 2
#define CREATE_CLIENT 3
#define DELETE_CLIENT 4
#define GET_VERSION 5

struct Request {
    int endpoint;
    string body;
    Request() {
        endpoint = -1;
        body = "";
    }
};

Request parse_req(string reqstr) {
    Request request;

    if (reqstr.rfind("GET /", 0) == 0) {
        request.endpoint = GET_NOTIFICATIONS;
        return request;
    }

    if (reqstr.rfind("POST /", 0) == 0)
        request.endpoint = CREATE_NOTIFICATION;

    if (reqstr.rfind("POST /client", 0) == 0) {
        request.endpoint = CREATE_CLIENT;
        return request;
    }

    if (reqstr.rfind("DELETE /client", 0) == 0) {
        request.endpoint = DELETE_CLIENT;
        return request;
    }

    if (reqstr.rfind("GET /version", 0) == 0) {
        request.endpoint = GET_VERSION;
        return request;
    }

    u32 cursor = 0;
    u32 count_nlines = 0;

    for (; cursor < reqstr.length() && count_nlines < 2; cursor++)
        if (reqstr[cursor] == 10)
            count_nlines++;
        else if (reqstr[cursor] != 13)
            count_nlines = 0;

    request.body = reqstr.erase(0, cursor);

    return request;
}

char *response(string headers, string body) {
    return headers.append(body).data();
}

string handle_req(sockaddr_in addr, Request request) {
    in_addr_t ip = addr.sin_addr.s_addr;

    switch (request.endpoint) {
    case GET_NOTIFICATIONS: {
        if (!clients.has(ip))
            return response(NOT_FOUND, "Client not registered\n");

        Client *client = clients.get(ip);

        string tmp = notifications.get_client_message(client);

        client->last_seen_now();

        return tmp.length() == 0 ? response(NO_CONTENT, tmp)
                                 : response(OK, tmp);
    };
    case CREATE_NOTIFICATION: {
        if (ip != localhost)
            return response(NOT_FOUND, "Not Found");

        if (request.body.length() == 0)
            return response(UNPROCESSABLE_CONTENT, "Unprocessable Content\n");

        notifications.push(Notification{request.body});
        return response(OK, "Notification created\n");
    };
    case CREATE_CLIENT: {
        if (!clients.has(ip)) {
            clients.push(Client{ip});

            return response(OK, "Client created\n");
        }

        return response(OK, "Client already exists\n");
    };
    case DELETE_CLIENT: {
        clients.remove(ip);

        return response(OK, "Client delete\n");
    };
    case GET_VERSION: {
        return response(OK, "notify v0.1.0\n");
    };
    default: {
        return response(NOT_FOUND, "Not Found\n");
    };
    }
}

int main() {
    unsigned short port = 5555;

    int fd;
    int sfd;
    int opt = 1;

    char buffer[4096];

    sockaddr_in serv_addr;
    sockaddr_in cli_addr;

    socklen_t socklen = sizeof(serv_addr);

    fd = socket(AF_INET, SOCK_STREAM, 0);

    if (fd == -1) {
        cerr << "socket failed" << endl;
        return 1;
    }

    if (setsockopt(fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt,
                   sizeof(opt))) {
        cerr << "setsocket failed" << endl;
        return 1;
    }

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = localhost;
    serv_addr.sin_port = htons(port);

    if (bind(fd, (sockaddr *)&serv_addr, socklen) < 0) {
        cout << "bind failed" << endl;
        return 1;
    }

    cout << "listening on " << port << endl;

    if (listen(fd, 32) == -1)
        close(fd);

    while (true) {
        sfd = accept(fd, (sockaddr *)&cli_addr, &socklen);

        if (sfd == -1) {
            cerr << "cannot open socket" << endl;

            continue;
        }

        bzero(buffer, 4096);

        if (read(sfd, buffer, 4095) < 0) {
            cerr << "cannot read response" << endl;
            close(sfd);

            continue;
        }

        string response = handle_req(cli_addr, parse_req(buffer));

        send(sfd, response.data(), response.length(), 0);

        close(sfd);
    }

    return 0;
}
