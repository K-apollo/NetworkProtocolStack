#ifndef SERVER_H
#define SERVER_H

#include <netinet/in.h> // 유닉스/리눅스용 소켓 헤더
#include <pthread.h>    // 스레드 라이브러리

// 클라이언트 정보 구조체
typedef struct 
{
    int socket;
    struct sockaddr_in address;
    socklen_t addr_len;
} client_info;

// 클라이언트 처리를 위한 함수 선언
void *handle_client(void *arg);

// 서버 시작 함수 선언
int start_server(const char *ip_address, int port);

#endif
