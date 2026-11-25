

// (1) connection

// (2) file transfer

// (3) get, put, list 

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <endian.h>

#define BACKLOG 1

// send helper
ssize_t send_bytes(int conn_fd, char * buf, int bytes){

    ssize_t current_len = 0, send_len = 0;
    while(current_len < bytes){
        send_len = send(conn_fd, buf+current_len, bytes-current_len, 0);
        if(send_len < 0) {
            perror("send_bytes() fails");
            exit(EXIT_FAILURE);
        }
        current_len += send_len;
    }
    return current_len;
}

// recv helper
ssize_t recv_bytes(int conn_fd, char * buf, int bytes){

    ssize_t current_len = 0, recv_len = 0;
    while(current_len < bytes){
        recv_len = recv(conn_fd, buf+current_len, bytes-current_len, 0);
        if(recv_len < 0) {
            perror("send_bytes() fails");
            exit(EXIT_FAILURE);
        }
        current_len += recv_len;
    }
    return current_len;
}


int main(int argc, char ** argv){

    if(argc != 2) {
        printf("Usage: ./client <portnum>");
        exit(EXIT_FAILURE);
    }

    int listen_fd = socket(AF_INET, SOCK_STREAM, 0);
    printf("listen_fd : %d\n", listen_fd);

    char * filename = argv[2];

    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_port = htons(atoi(argv[1]));
    // addr.sin_addr (struct in_addr)

    if (inet_aton("127.0.0.1", &addr.sin_addr) == 0) {
        fprintf(stderr, "inet_aton() fails");
        exit(EXIT_FAILURE);
    }
    if (bind(listen_fd, (struct sockaddr *) &addr, sizeof(addr)) == -1) {
        perror("bind() fails");
        exit(EXIT_FAILURE);
    }
    if (listen(listen_fd, BACKLOG) == -1) {
        perror("listen() fails");
        exit(EXIT_FAILURE);
    }

    int conn_fd = accept(listen_fd, NULL, NULL);

    uint64_t recv_len;
    recv_bytes(conn_fd, (char *) &recv_len, 8);
    uint64_t file_len = be64toh(recv_len);

    char * content = malloc(sizeof(char) * file_len + 1);
    recv_bytes(conn_fd, content, file_len);
    printf("recv file content : \n%s", content);


    exit(EXIT_SUCCESS);
}