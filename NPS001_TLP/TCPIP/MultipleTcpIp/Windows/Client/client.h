#ifndef CLIENT_H
#define CLIENT_H

#include <winsock2.h>
#include <ws2tcpip.h>

#pragma comment(lib, "ws2_32.lib")

// 클라이언트 시작 함수 선언
int start_client(const char *ip_address, int port);

#endif
