#include "notification.hpp"
#include "config.cpp"
#include <ctime>
#include <fstream>
#include <iostream>
#include <sstream>

Notification::Notification() {}

Notification::Notification(string _message) {
    date = time(0);
    message = sanitize(_message);
}

Notification::Notification(time_t _date, string _message) {
    date = _date;
    message = _message;
}

string Notification::sanitize(string raw) {
    size_t pos;

    while ((pos = raw.find("|")) != string::npos)
        raw.replace(pos, 1, "-");

    while ((pos = raw.find(";")) != string::npos)
        raw.replace(pos, 1, ",");

    return raw;
}

string Notification::to_string() {
    return std::to_string(date) + "|" + message + ";\n";
}

void Notification::save() {
    ofstream f;
    f.open(NOTIFICATION_DATABASE, ios_base::app);
    f << to_string();
}

NotificationsList::NotificationsList() {
    init = false;
    data = new Notification[16];
    length = 0;
    capacity = 16;

    load();
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
        notification.save();
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

void NotificationsList::load() {
    time_t now = time(0);

    ifstream f(NOTIFICATION_DATABASE);

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
                push(Notification{date, notif[1]});

            notif[0] = "";
            notif[1] = "";
        } else
            notif[t] += data[i];
    }
}
