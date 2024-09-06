#ifndef SERVER_H
#define SERVER_H

#include <winsock2.h>
#include <ws2tcpip.h>
#include <pthread.h>

#pragma comment(lib, "ws2_32.lib")

// 클라이언트 정보 구조체
typedef struct {
    SOCKET socket;
    struct sockaddr_in address;
    int addr_len;
} client_info;

// 클라이언트 처리를 위한 함수 선언
void *handle_client(void *arg);

// 서버 시작 함수 선언
int start_server(const char *ip_address, int port);

#endif
