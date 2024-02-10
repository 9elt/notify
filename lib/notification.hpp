#include "client.hpp"
#include <ctime>
#include <string>

using namespace std;

typedef unsigned int u32;

struct Notification {
    time_t date;
    string message;
    Notification();
    Notification(string _message);
};

struct NotificationsList {
    Notification *data;
    u32 length;
    u32 capacity;
    NotificationsList();
    void resize(u32 size);
    void push(Notification notification);
    void clear_read(time_t before);
    string get_client_message(Client *client);
};
