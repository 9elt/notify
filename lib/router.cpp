#include "router.hpp"

ClientsList clients;

NotificationsList notifications;

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
        if (ip != LOCALHOST)
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
