#include "request.hpp"

Request::Request(string reqstr) {
    if (reqstr.rfind("GET /version ", 0) == 0) {
        endpoint = GET_VERSION;
        return;
    }

    if (reqstr.rfind("GET / ", 0) == 0) {
        endpoint = GET_NOTIFICATIONS;
        return;
    }

    if (reqstr.rfind("POST / ", 0) == 0)
        endpoint = CREATE_NOTIFICATION;

    u32 cursor = 0;
    u32 count_nlines = 0;

    for (; cursor < reqstr.length() && count_nlines < 2; cursor++)
        if (reqstr[cursor] == 10)
            count_nlines++;
        else if (reqstr[cursor] != 13)
            count_nlines = 0;

    body = reqstr.erase(0, cursor);

    return;
}
