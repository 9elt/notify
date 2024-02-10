#include "types.hpp"
#include <ctime>
#include <netinet/in.h>

struct Client {
    time_t last_seen;
    in_addr_t ip;
    Client();
    Client(in_addr_t ip);
    Client(time_t last_seen, in_addr_t ip);
    void last_seen_now();
    string to_string();
};

struct ClientsList {
    Client *data;
    u32 length;
    u32 capacity;
    bool init;
    ClientsList();
    void resize(u32 size);
    void push(Client client);
    void remove(in_addr_t ip);
    bool has(in_addr_t ip);
    Client *get(in_addr_t ip);
    Client *oldest();
    string to_string();
};
