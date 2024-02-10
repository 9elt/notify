#include "client.hpp"

Client::Client() {}

Client::Client(in_addr_t _ip) {
    last_seen = time(0);
    ip = _ip;
}

void Client::last_seen_now() { last_seen = time(0); }

ClientsList::ClientsList() {
    data = new Client[1];
    length = 0;
    capacity = 1;
}

void ClientsList::resize(u32 size) {
    Client *tmp = new Client[size];

    for (u32 i = 0; i < length; i++)
        tmp[i] = data[i];

    delete[] data;

    data = tmp;
    capacity = size;
}

void ClientsList::push(Client client) {
    if (length == capacity)
        resize(capacity * 2);

    data[length++] = client;
}

void ClientsList::remove(in_addr_t ip) {
    if (has(ip)) {
        Client *tmp = new Client[capacity];

        for (u32 i = 0; i < length; i++)
            if (data[i].ip != ip)
                tmp[i] = data[i];

        delete[] data;
        data = tmp;
        length--;
    }
}

bool ClientsList::has(in_addr_t ip) {
    for (u32 i = 0; i < length; i++)
        if (ip == data[i].ip)
            return true;

    return false;
}

Client *ClientsList::get(in_addr_t ip) {
    for (u32 i = 0; i < length; i++)
        if (ip == data[i].ip)
            return &data[i];

    return nullptr;
}

Client *ClientsList::oldest() {
    if (length == 0)
        return nullptr;

    Client *oldest = &data[0];

    for (u32 i = 0; i < length; i++)
        if (data[i].last_seen < oldest->last_seen)
            oldest = &data[i];

    return oldest;
}
