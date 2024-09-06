#include <stdio.h>
#include <stdlib.h>
#include "client.h"

int main(int argc, char *argv[]) {
    if (argc != 3) {
        fprintf(stderr, "Usage: %s <IP> <Port>\n", argv[0]);
        return EXIT_FAILURE;
    }

    char *ip_address = argv[1];
    int port = atoi(argv[2]);

    // 클라이언트 시작
    if (start_client(ip_address, port) == -1) {
        fprintf(stderr, "Failed to connect to server.\n");
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}
