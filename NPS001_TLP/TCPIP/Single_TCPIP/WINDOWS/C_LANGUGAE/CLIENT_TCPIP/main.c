#include "tcp_client.h"

int main() {
    const char *ip_address = "127.0.0.1";
    int port = 8080;
    connect_to_server(ip_address, port);
    return 0;
}