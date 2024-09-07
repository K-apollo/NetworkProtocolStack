#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include "client.h"

#define BUFFER_SIZE 1024

int start_client(const char *ip_address, int port) {
    int sock;
    struct sockaddr_in server_addr;
    char buffer[BUFFER_SIZE];

    // 소켓 생성
    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
        perror("Socket failed");
        return -1;
    }

    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = inet_addr(ip_address);
    server_addr.sin_port = htons(port);

    // 서버에 연결
    if (connect(sock, (struct sockaddr*)&server_addr, sizeof(server_addr)) == -1) {
        perror("Connect failed");
        close(sock);
        return -1;
    }

    printf("Connected to server at %s:%d\n", ip_address, port);

    // 메시지 송수신 반복
    while (1) {
        printf("Enter message: ");
        fgets(buffer, BUFFER_SIZE, stdin);
        send(sock, buffer, strlen(buffer), 0);

        ssize_t nbytes = recv(sock, buffer, BUFFER_SIZE, 0);
        if (nbytes <= 0) {
            perror("Receive failed");
            break;
        }
        buffer[nbytes] = '\0';
        printf("Received from server: %s\n", buffer);
    }

    close(sock);
    return 0;
}
