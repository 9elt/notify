#include <ctime>
#include <netinet/in.h>

typedef unsigned int u32;

struct Client {
    time_t last_seen;
    in_addr_t ip;
    Client();
    Client(in_addr_t _ip);
    void last_seen_now();
};

struct ClientsList {
    Client *data;
    u32 length;
    u32 capacity;
    ClientsList();
    void resize(u32 size);
    void push(Client client);
    void remove(in_addr_t ip);
    bool has(in_addr_t ip);
    Client *get(in_addr_t ip);
    Client *oldest();
};
