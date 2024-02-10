#include "client.cpp"
#include <ctime>
#include <string>

using namespace std;

typedef unsigned int u32;

struct Notification {
    time_t date;
    string message;
    Notification() {}
    Notification(string _message) {
        date = time(0);
        message = _message;
    }
};

struct NotificationsList {
    Notification *data;
    u32 length;
    u32 capacity;

    NotificationsList() {
        data = new Notification[1];
        length = 0;
        capacity = 1;
    }

    void resize(u32 size) {
        Notification *tmp = new Notification[size];

        for (u32 i = 0; i < length; i++)
            tmp[i] = data[i];

        delete[] data;

        data = tmp;
        capacity = size;
    }

    void push(Notification notification) {
        if (length == capacity)
            resize(capacity * 2);

        data[length++] = notification;
    }

    void clear_read(time_t before) {
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

    string get_client_message(Client *client) {
        time_t last_seen = client->last_seen;

        string res = "";

        for (u32 i = 0; i < length; i++)
            if (data[i].date > last_seen) {
                res.append(data[i].message);
                res.append("\n");
            }

        return res;
    }
};
