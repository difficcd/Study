

// (1) connection

// (2) file transfer

// (3) get, put, list 

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <endian.h>

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

    if(argc != 3) {
        printf("Usage: ./server <portnum> <file>\n");
        exit(EXIT_FAILURE);
    }

    int conn_fd = socket(AF_INET, SOCK_STREAM, 0);
    
    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_port = htons(atoi(argv[1]));
    // addr.sin_addr (struct in_addr)

    if (inet_aton("127.0.0.1", &addr.sin_addr) == 0) {
        fprintf(stderr, "inet_aton() fails");
        exit(EXIT_FAILURE);
    }
    if (connect(conn_fd, (struct sockaddr *) &addr, sizeof(addr)) == -1){
        perror("connect() fails");
        exit(EXIT_FAILURE);
    }


    char * filepath = argv[2];

    // file len (stat, st_size)
    struct stat s;
    if(stat(filepath, &s) < 0){
        perror("stat() fails");
        exit(EXIT_FAILURE);
    }
    uint64_t file_len = (uint64_t) s.st_size;

    // file contents (fopen + fread + string send : '\0')
    char * content = malloc(sizeof(char) * file_len + 1);
    FILE * fp = fopen(filepath, "rb");
    fread(content, 1, file_len, fp);
    content[file_len] = '\0';

    printf("file content : \n%s", content);


    // send
    uint64_t send_len = htobe64(file_len);
    send_bytes(conn_fd, (char *) &send_len, 8);
    send_bytes(conn_fd,     content, file_len);
    

    exit(EXIT_SUCCESS);
}