#include "tcp_client.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

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

// 메모리 해제를 위한 함수
void safe_free(void *ptr) {
    if (ptr != NULL) {
        free(ptr);
        ptr = NULL;
    }
}

int connect_to_server(const char *ip_address, int port)
{
    int sock = 0;
    struct sockaddr_in serv_addr;

    // 동적 메모리 할당을 사용한 버퍼 생성
    char *buffer = safe_malloc(BUFFER_SIZE);
    const char *hello = "Hello from client";

    // 소켓 생성
    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) 
    {
        perror("Socket creation error");
        safe_free(buffer); // 오류 시 메모리 해제
        return -1;
    }

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(port);

    // 서버 주소 설정
    if (inet_pton(AF_INET, ip_address, &serv_addr.sin_addr) <= 0) 
    {
        printf("Invalid address\n");
        safe_free(buffer); // 오류 시 메모리 해제
        close(sock);
        return -1;
    }

    // 서버에 연결
    if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) 
    {
        printf("Connection failed\n");
        safe_free(buffer); // 오류 시 메모리 해제
        close(sock);
        return -1;
    }

    // 서버로 데이터 전송 및 응답 수신
    send(sock, hello, strlen(hello), 0);
    printf("Hello message sent\n");
    read(sock, buffer, BUFFER_SIZE);
    printf("Message from server: %s\n", buffer);

    // 소켓 및 메모리 해제
    close(sock);
    safe_free(buffer);

    return 0;
}