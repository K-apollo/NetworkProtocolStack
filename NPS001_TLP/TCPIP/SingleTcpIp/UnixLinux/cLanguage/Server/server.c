#include "tcp_server.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>

#define BUFFER_SIZE 1024

// 메모리 할당과 해제 관리를 위한 유틸리티 함수
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

void safe_free(void *ptr) 
{
    if (ptr != NULL) 
    {
        free(ptr);
        ptr = NULL;
    }
}

int start_server(int port) {
    int server_fd, new_socket;
    struct sockaddr_in address;
    int addrlen = sizeof(address);

    // 동적 메모리 할당을 사용한 버퍼 생성
    char *buffer = safe_malloc(BUFFER_SIZE);
    const char *hello = "Hello from server";

    // 소켓 생성
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
        perror("socket failed");
        safe_free(buffer); // 오류 시 메모리 해제
        return -1;
    }

    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(port);

    // 소켓 바인딩
    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0) 
    {
        perror("bind failed");
        close(server_fd);
        safe_free(buffer); // 오류 시 메모리 해제
        return -1;
    }

    // 연결 대기
    if (listen(server_fd, 3) < 0) 
    {
        perror("listen failed");
        close(server_fd);
        safe_free(buffer); // 오류 시 메모리 해제
        return -1;
    }

    // 클라이언트 연결 수락
    if ((new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t*)&addrlen)) < 0) 
    {
        perror("accept failed");
        close(server_fd);
        safe_free(buffer); // 오류 시 메모리 해제
        return -1;
    }

    // 클라이언트로부터 데이터 수신 및 응답 전송
    read(new_socket, buffer, BUFFER_SIZE);
    printf("Message from client: %s\n", buffer);
    send(new_socket, hello, strlen(hello), 0);
    printf("Hello message sent\n");

    // 소켓 및 메모리 해제
    close(new_socket);
    close(server_fd);
    safe_free(buffer);

    return 0;
}