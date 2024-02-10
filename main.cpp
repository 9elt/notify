#include "router.hpp"
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
    serv_addr.sin_addr.s_addr = LOCALHOST;
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

        string response = handle_req(cli_addr, Request(buffer));

        send(sfd, response.data(), response.length(), 0);

        close(sfd);
    }

    return 0;
}
