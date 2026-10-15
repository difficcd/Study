#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT 8080
#define BUFFER_SIZE 1024

int main() {
    int sock = 0;
    struct sockaddr_in serv_addr;
    // socket address internet : IPv4 전용 소켓주소정보 담는 C언어 구조체
    /*
    struct sockaddr_in {
    sa_family_t    sin_family; // 주소 체계 (IPv4냐 IPv6냐)
    in_port_t      sin_port;   // 포트 번호
    struct in_addr sin_addr;   // IP 주소
    char           sin_zero[8];// 크기 맞춤용 여분 공간 (패딩)
    };
    */

    char * message = "Hello from client";
    char buffer[BUFFER_SIZE] = {0};

    sock = socket(AF_INET, SOCK_STREAM, 0);
    // AF_INET == Address Family - INETernet
    // IPv4 주소체계 사용하겠다고 컴파일러, OS에게 알려주는 정수 상수(매크로)
    // SOCK_STREAM == TCP. connect, accept로 회선부터연결, 안정성, 데이터는 Byte Stream (끊어읽기가능)
    // SOCK_DGRAM == UDP. connect, accept 없음, 신뢰성낮음, 빠름. 

    if (sock < 0) {
        perror("socket creation error");
        return -1;
    }

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT);

    if (inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr) <= 0) {
        perror("invalid address");
        return -1;
    }

    if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) {
        // int connect(int sockfd, const struct sockaddr *addr, socklen_t addrlen);
        perror("connection failed");
        return -1;
    }

    send(sock, message, strlen(message), 0);
    read(sock, buffer, BUFFER_SIZE);
    // ssize_t send(int sockfd, const void *buf, size_t len, int flags);


    printf("Received: %s\n", buffer);

    close(sock);
    // int close(int fd);
    return 0;
}