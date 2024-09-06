#include "tcp_server.h"
#include <stdio.h>
#include <winsock2.h>
#include <stdlib.h>

#pragma comment(lib, "ws2_32.lib") // Winsock 라이브러리 링크

#define BUFFER_SIZE 1024

// 안전한 메모리 할당 함수
char* safe_malloc(size_t size) {
    char *ptr = (char*)malloc(size);
    if (ptr == NULL) {
        perror("Memory allocation failed");
        exit(EXIT_FAILURE); // 메모리 할당 실패 시 프로그램 종료
    }
    return ptr;
}

// 메모리 해제 함수
void safe_free(void *ptr) {
    if (ptr != NULL) {
        free(ptr);
        ptr = NULL;
    }
}

int start_server(int port) {
    WSADATA wsa;
    SOCKET server_socket, client_socket;
    struct sockaddr_in server, client;
    int client_len;
    // 동적 메모리 할당을 사용한 버퍼 생성
    char *buffer = safe_malloc(BUFFER_SIZE);
    const char *message = "Hello from server";

    // Winsock 초기화
    if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0) {
        printf("Failed. Error Code : %d\n", WSAGetLastError());
        safe_free(buffer); // 메모리 해제
        return -1;
    }

    // 소켓 생성
    if ((server_socket = socket(AF_INET, SOCK_STREAM, 0)) == INVALID_SOCKET) {
        printf("Could not create socket : %d\n", WSAGetLastError());
        WSACleanup();
        safe_free(buffer); // 메모리 해제
        return -1;
    }

    server.sin_family = AF_INET;
    server.sin_addr.s_addr = INADDR_ANY;
    server.sin_port = htons(port);

    // 소켓 바인딩
    if (bind(server_socket, (struct sockaddr *)&server, sizeof(server)) == SOCKET_ERROR) {
        printf("Bind failed with error code : %d\n", WSAGetLastError());
        closesocket(server_socket);
        WSACleanup();
        safe_free(buffer); // 메모리 해제
        return -1;
    }

    // 연결 대기
    listen(server_socket, 3);

    client_len = sizeof(struct sockaddr_in);
    client_socket = accept(server_socket, (struct sockaddr *)&client, &client_len);
    if (client_socket == INVALID_SOCKET) {
        printf("Accept failed with error code : %d\n", WSAGetLastError());
        closesocket(server_socket);
        WSACleanup();
        safe_free(buffer); // 메모리 해제
        return -1;
    }

    recv(client_socket, buffer, BUFFER_SIZE, 0);
    printf("Message from client: %s\n", buffer);

    send(client_socket, message, strlen(message), 0);
    printf("Message sent\n");

    // 소켓 및 메모리 해제
    closesocket(client_socket);
    closesocket(server_socket);
    WSACleanup();
    safe_free(buffer); // 메모리 해제

    return 0;
}