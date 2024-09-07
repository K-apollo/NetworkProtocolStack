#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include "server.h"

#define BUFFER_SIZE 1024
#define MAX_CLIENTS 10

client_info clients[MAX_CLIENTS];
pthread_mutex_t clients_mutex = PTHREAD_MUTEX_INITIALIZER;

void *handle_client(void *arg) 
{
    client_info *cli = (client_info *)arg;
    char buffer[BUFFER_SIZE];
    ssize_t nbytes;

    // 클라이언트로부터 메시지 수신 및 에코 처리
    while ((nbytes = recv(cli->socket, buffer, BUFFER_SIZE, 0)) > 0) {
        buffer[nbytes] = '\0';
        printf("Received from client: %s\n", buffer);
        send(cli->socket, buffer, nbytes, 0);
    }

    // 클라이언트 소켓 종료
    close(cli->socket);
    pthread_mutex_lock(&clients_mutex);
    cli->socket = 0;
    pthread_mutex_unlock(&clients_mutex);

    free(cli);
    pthread_exit(NULL);
}

int start_server(const char *ip_address, int port) 
{
    int server_socket, new_socket;
    struct sockaddr_in server_addr, client_addr;
    socklen_t addr_len = sizeof(client_addr);

    // 소켓 생성
    if ((server_socket = socket(AF_INET, SOCK_STREAM, 0)) == -1) 
    {
        perror("Socket failed");
        return -1;
    }

    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = inet_addr(ip_address);
    server_addr.sin_port = htons(port);

    // 소켓 바인딩
    if (bind(server_socket, (struct sockaddr*)&server_addr, sizeof(server_addr)) == -1) 
    {
        perror("Bind failed");
        close(server_socket);
        return -1;
    }

    // 연결 대기 상태로 전환
    if (listen(server_socket, MAX_CLIENTS) == -1) 
    {
        perror("Listen failed");
        close(server_socket);
        return -1;
    }

    printf("Server listening on %s:%d\n", ip_address, port);

    while (1) 
    {
        // 클라이언트 연결 수락
        new_socket = accept(server_socket, (struct sockaddr*)&client_addr, &addr_len);
        if (new_socket == -1) 
        {
            perror("Accept failed");
            continue;
        }

        pthread_mutex_lock(&clients_mutex);
        client_info *cli = malloc(sizeof(client_info));
        cli->socket = new_socket;
        cli->address = client_addr;
        cli->addr_len = addr_len;

        // 클라이언트 처리 스레드 생성
        pthread_t tid;
        pthread_create(&tid, NULL, handle_client, (void *)cli);
        pthread_detach(tid);
        pthread_mutex_unlock(&clients_mutex);
    }

    close(server_socket);
    return 0;
}
