
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>

#include <arpa/inet.h>
#include <netinet/in.h>


int main () {

    int fd = socket(AF_INET, SOCK_STREAM, 0);
    printf("server fd : %d \n", fd);

    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_port = htons(12199);

    if (inet_aton("127.0.0.1", &(addr.sin_addr)) == 0) {
        perror("inet_aton() fails");
        exit(EXIT_FAILURE);
    }

    if (bind(fd, (struct sockaddr *) &addr, sizeof(addr)) == -1) {
        perror("bind() fails");
        exit(EXIT_FAILURE);
    }

    if (listen(fd, 1) == -1) {
        perror("listen() fails");
        exit(EXIT_FAILURE);
    }

    accept(fd, NULL, NULL);

    while(1) {};

}