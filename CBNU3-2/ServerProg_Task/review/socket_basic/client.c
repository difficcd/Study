
#include <stdio.h>
#include <stdlib.h>

#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>

int main () {

    int fd = socket(AF_INET, SOCK_STREAM, 0);
    printf("fd : %d \n", fd);

    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_port = htons(12199); 

    if (inet_aton("127.0.0.1", &(addr.sin_addr)) == 0){
        perror("inet_aton() fails");
        exit(EXIT_FAILURE);
    }

    if (connect(fd, (const struct sockaddr *) &addr, sizeof(addr)) == -1){
        perror("connect() fails");
        exit(EXIT_FAILURE);
    }

    while(1){};

}