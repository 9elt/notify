#include "client.hpp"
#include <ctime>
#include <string>

struct Notification {
    time_t date;
    string message;
    Notification();
    Notification(string message);
    Notification(time_t time, string message);
    string sanitize(string raw);
    string to_string();
    void save();
};

struct NotificationsList {
    Notification *data;
    u32 length;
    u32 capacity;
    bool init;
    NotificationsList();
    void resize(u32 size);
    void push(Notification notification);
    string get_client_message(Client *client);
    string to_string();
    void load();
};
