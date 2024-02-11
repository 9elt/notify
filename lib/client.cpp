#include "client.hpp"
#include "config.cpp"
#include <ctime>
#include <fstream>
#include <iostream>
#include <sstream>

Client::Client() {}

Client::Client(in_addr_t _ip) {
    last_seen = time(0);
    ip = _ip;
}

Client::Client(time_t _last_seen, in_addr_t _ip) {
    last_seen = _last_seen;
    ip = _ip;
}

void Client::last_seen_now() {
    last_seen = time(0);
    save();
}

string Client::to_string() {
    return std::to_string(last_seen) + "|" + std::to_string(ip) + ";\n";
}

void Client::save() {
    ofstream f;
    f.open(CLIENTS_DATABASE, ios_base::app);
    f << to_string();
}

ClientsList::ClientsList() {
    init = false;
    data = new Client[1];
    length = 0;
    capacity = 1;

    load();
    init = true;
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

    if (init)
        client.save();
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

string ClientsList::to_string() {
    string res;

    for (u32 i = 0; i < length; i++)
        res.append(data[i].to_string());

    return res;
}

void ClientsList::load() {
    ifstream f(CLIENTS_DATABASE);

    if (!f)
        return;

    stringstream buffer;
    buffer << f.rdbuf();

    string data = buffer.str();

    int t = 0;

    string client[2];

    for (u32 i = 0; i < data.length(); i++) {
        auto c = data[i];
        if (c == *"|")
            t++;
        else if (c == *";") {
            i++;
            t--;

            time_t last_seen = (time_t)stoi(client[0]);
            in_addr_t ip = (in_addr_t)stoi(client[1]);

            if (has(ip))
                get(ip)->last_seen = last_seen;
            else
                push(Client{last_seen, ip});

            client[0] = "";
            client[1] = "";
        } else
            client[t] += data[i];
    }
}
