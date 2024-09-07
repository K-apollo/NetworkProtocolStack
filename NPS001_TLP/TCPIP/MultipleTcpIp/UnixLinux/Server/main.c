#include <stdio.h>
#include <stdlib.h>
#include "server.h"

int main(int argc, char *argv[]) {
    if (argc != 3) {
        fprintf(stderr, "Usage: %s <IP> <Port>\n", argv[0]);
        return EXIT_FAILURE;
    }

    char *ip_address = argv[1];
    int port = atoi(argv[2]);

    // 서버 시작
    if (start_server(ip_address, port) == -1) {
        fprintf(stderr, "Failed to start server.\n");
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}
