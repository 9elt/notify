#include "notification.hpp"
#include <ctime>
#include <fstream>
#include <iostream>
#include <sstream>

string const DATABASE = "./data/notifications";

time_t const MAX_TIME = 2764800;

void read_file(NotificationsList *list) {
    time_t now = time(0);

    ifstream f(DATABASE);

    if (!f)
        return;

    stringstream buffer;
    buffer << f.rdbuf();

    string data = buffer.str();

    int t = 0;

    string notif[2];

    for (u32 i = 0; i < data.length(); i++) {
        auto c = data[i];
        if (c == *"|")
            t++;
        else if (c == *";") {
            i++;
            t--;
            time_t date = stoi(notif[0]);

            if (now - date < MAX_TIME)
                list->push(Notification{date, notif[1]});

            notif[0] = "";
            notif[1] = "";
        } else
            notif[t] += data[i];
    }
}

void push_file(Notification *notification) {
    ofstream f;
    f.open(DATABASE, ios_base::app);
    f << notification->to_string();
}

string sanitize(string raw) {
    size_t pos;

    while ((pos = raw.find("|")) != string::npos)
        raw.replace(pos, 1, "-");

    while ((pos = raw.find(";")) != string::npos)
        raw.replace(pos, 1, ",");

    return raw;
}

Notification::Notification() {}

Notification::Notification(string _message) {
    date = time(0);
    message = sanitize(_message);
}

Notification::Notification(time_t _date, string _message) {
    date = _date;
    message = _message;
}

string Notification::to_string() {
    return std::to_string(date) + "|" + message + ";\n";
}

NotificationsList::NotificationsList() {
    init = false;
    data = new Notification[16];
    length = 0;
    capacity = 16;

    read_file(this);
    init = true;
}

void NotificationsList::resize(u32 size) {
    Notification *tmp = new Notification[size];

    for (u32 i = 0; i < length; i++)
        tmp[i] = data[i];

    delete[] data;

    data = tmp;
    capacity = size;
}

void NotificationsList::push(Notification notification) {
    if (length == capacity)
        resize(capacity * 2);

    data[length++] = notification;

    if (init)
        push_file(&notification);
}

void NotificationsList::clear_read(time_t before) {
    Notification tmp[length];
    u32 nlen = 0;

    for (u32 i = 0; i < length; i++)
        if (data[i].date > before)
            tmp[nlen++] = data[i];

    delete[] data;
    data = new Notification[nlen];

    for (u32 i = 0; i < nlen; i++)
        data[i] = tmp[i];

    length = nlen;
}

string NotificationsList::get_client_message(Client *client) {
    time_t last_seen = client->last_seen;

    string res = "";

    for (u32 i = 0; i < length; i++)
        if (data[i].date > last_seen) {
            res.append(data[i].message);
            res.append("\n\n");
        }

    return res;
}

string NotificationsList::to_string() {
    string res;

    for (u32 i = 0; i < length; i++)
        res.append(data[i].to_string());

    return res;
}
