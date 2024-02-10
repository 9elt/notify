#include <arpa/inet.h>
#include <netinet/in.h>
#include <string>

using namespace std;

in_addr_t const LOCALHOST = inet_addr("127.0.0.1");

string const NOT_FOUND = "HTTP/1.1 404\nContent-Type: text/plain\n\n";

string const UNPROCESSABLE_CONTENT =
    "HTTP/1.1 422\nContent-Type: text/plain\n\n";

string const NO_CONTENT = "HTTP/1.1 204\nContent-Type: text/plain\n\n";

string const OK = "HTTP/1.1 200\nContent-Type: text/plain\n\n";
