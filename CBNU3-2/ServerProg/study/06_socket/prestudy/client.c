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
    char *message = "Hello from client";
    char buffer[BUFFER_SIZE] = {0};

    sock = socket(AF_INET, SOCK_STREAM, 0);


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