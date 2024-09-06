#include "tcp_client.h"
#include <stdio.h>
#include <winsock2.h>
#include <stdlib.h>

#pragma comment(lib, "ws2_32.lib") // Winsock 라이브러리 링크

#define BUFFER_SIZE 1024

// 안전한 메모리 할당 함수
char* safe_malloc(size_t size) 
{
    char *ptr = (char*)malloc(size);
    if (ptr == NULL) 
    {
        perror("Memory allocation failed");
        exit(EXIT_FAILURE); // 메모리 할당 실패 시 프로그램 종료
    }
    return ptr;
}

// 메모리 해제를 위한 함수
void safe_free(void *ptr) 
{
    if (ptr != NULL) 
    {
        free(ptr);
        ptr = NULL;
    }
}

int connect_to_server(const char *ip_address, int port) 
{
    WSADATA wsa;
    SOCKET sock;
    struct sockaddr_in server;
    
    // 동적 메모리 할당을 사용한 버퍼 생성
    char *server_reply = safe_malloc(BUFFER_SIZE);
    const char *message = "Hello from client";

    // Winsock 초기화
    if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0) 
    {
        printf("Failed. Error Code : %d\n", WSAGetLastError());
        safe_free(server_reply); // 메모리 해제
        return -1;
    }

    // 소켓 생성
    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) == INVALID_SOCKET) 
    {
        printf("Could not create socket : %d\n", WSAGetLastError());
        WSACleanup();
        safe_free(server_reply); // 메모리 해제
        return -1;
    }

    server.sin_addr.s_addr = inet_addr(ip_address);
    server.sin_family = AF_INET;
    server.sin_port = htons(port);

    // 서버에 연결
    if (connect(sock, (struct sockaddr *)&server, sizeof(server)) < 0) 
    {
        printf("Connection error\n");
        closesocket(sock);
        WSACleanup();
        safe_free(server_reply); // 메모리 해제
        return -1;
    }

    // 서버로 메시지 전송
    send(sock, message, strlen(message), 0);
    printf("Message sent\n");

    // 서버로부터 응답 수신
    recv(sock, server_reply, BUFFER_SIZE, 0);
    printf("Server reply: %s\n", server_reply);

    // 소켓 및 메모리 해제
    closesocket(sock);
    WSACleanup();
    safe_free(server_reply); // 메모리 해제

    return 0;
}