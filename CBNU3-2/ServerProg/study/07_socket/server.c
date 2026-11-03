#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT 8080
#define BUFFER_SIZE 1024

int main() {
    int server_fd, client_socket;

    struct sockaddr_in address;
    int addrlen = sizeof(address);

    char buffer[BUFFER_SIZE] = {0};
    char *message = "Hello from server";

    server_fd = socket(AF_INET, SOCK_STREAM, 0);
    // int socket(int domain, int type, int protocol);
    // domain: AF_INET (IPv4) 또는 AF_INET6 (IPv6) 
    // type: SOCK_STREAM (TCP) 또는 SOCK_DGRAM (UDP)
    // protocol: 보통 0 (기본 프로토콜)

    if (server_fd == -1) {
        perror("socket failed");
        // sys call, c lib func 실행 실패 시 
        // errno 에 저장된 에러 번호를 문장으로 변환해 stderr에 출력
        exit(EXIT_FAILURE);
    }

    int opt = 1;
    setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY; // 모든 IP 수신
    address.sin_port = htons(PORT);       // 포트번호 (네트워크 바이트 순서)

    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0) {
        // int bind(int sockfd, const struct sockaddr *addr, socklen_t addrlen);
    
        perror("bind failed"); // 주소/포트 중복 사용 중일 때 주 원인 출력
        close(server_fd);
        exit(EXIT_FAILURE);
    }

    if (listen(server_fd, 3) < 0) {
        // 클라이언트 접속 요청 받을 대기 큐 생성
        // int listen(int sockfd, int backlog);
        // 인자: backlog (동시 대기 가능한 클라이언트 연결 요청 큐의 크기)

        perror("listen failed");
        close(server_fd);
        exit(EXIT_FAILURE);
    }

    client_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t*)&addrlen);
    // int accept(int sockfd, struct sockaddr *addr, socklen_t *addrlen);

    if (client_socket < 0) {
        perror("accept failed");
        // 개별 클라이언트 수락 실패 시 서버 전체를 끄지 않고 계속 진행할 수도 있음
        
        close(server_fd);
        exit(EXIT_FAILURE);
    }

    read(client_socket, buffer, BUFFER_SIZE);
    printf("Received: %s\n", buffer);

    send(client_socket, message, strlen(message), 0);

    close(client_socket);
    close(server_fd);
    return 0;
}