#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <winsock2.h>
#include <ws2tcpip.h>
#include "client.h"

#define BUFFER_SIZE 1024

int start_client(const char *ip_address, int port) {
    WSADATA wsa;
    SOCKET sock;
    struct sockaddr_in server_addr;
    char buffer[BUFFER_SIZE];

    // Winsock2 초기화
    if (WSAStartup(MAKEWORD(2,2),&wsa) != 0) {
        printf("Failed to initialize Winsock. Error Code : %d",WSAGetLastError());
        return -1;
    }

    // 소켓 생성
    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) == INVALID_SOCKET) {
        printf("Could not create socket : %d", WSAGetLastError());
        WSACleanup();
        return -1;
    }

    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = inet_addr(ip_address);
    server_addr.sin_port = htons(port);

    // 서버에 연결
    if (connect(sock, (struct sockaddr*)&server_addr, sizeof(server_addr)) == SOCKET_ERROR) {
        printf("Connect failed with error code : %d", WSAGetLastError());
        closesocket(sock);
        WSACleanup();
        return -1;
    }

    printf("Connected to server at %s:%d\n", ip_address, port);

    // 메시지 송수신 반복
    while (1) {
        printf("Enter message: ");
        fgets(buffer, BUFFER_SIZE, stdin);
        send(sock, buffer, strlen(buffer), 0);

        int nbytes = recv(sock, buffer, BUFFER_SIZE, 0);
        if (nbytes == SOCKET_ERROR || nbytes == 0) {
            printf("Receive failed or connection closed\n");
            break;
        }
        buffer[nbytes] = '\0';
        printf("Received from server: %s\n", buffer);
    }

    closesocket(sock);
    WSACleanup();
    return 0;
}
