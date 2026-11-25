#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <pthread.h>


int fd;
pthread_t recv_thread;
pthread_t send_thread;

// send helper
ssize_t send_bytes(int conn_fd, char * buf, int byte) {

    ssize_t current_len = 0, send_len = 0;
    while(current_len < byte) {
        send_len = send(conn_fd, buf+current_len, byte-current_len, 0);

        if(send_len == 0){
            return 0;  // connection refused 
        }
        else if (send_len <= -1) {
            perror("send_bytes() fails");
            return -1; // error
        }
        current_len += send_len;
    }
}

// recv helper
ssize_t recv_bytes(int conn_fd, char * buf, int byte) {

    ssize_t current_len = 0, recv_len = 0;
    while(current_len < byte) {
        recv_len = recv(conn_fd, buf+current_len, byte-current_len, 0);

        if(recv_len == 0){
            return 0;  // connection refused 
        }
        else if (recv_len <= -1) {
            perror("send_bytes() fails");
            return -1; // error
        }
        current_len += recv_len;
    }
}

// return 1 == return NULL(thread exit)
int check(int res, int conn_fd){
    if (res >  0) {
        //printf("res bytes : %d\n", res);
        return 0;
    }
    else if (res <  0)    perror("check fails");
    else if (res == 0)    printf("connection refused\n");
    return 1;
}

int main(int argc, char ** argv) {
    
    if ((fd = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
        perror("socket() fails");
        exit(EXIT_FAILURE);
    }
    printf("fd: %d\n", fd);

    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_port = htons(atoi(argv[1]));

    // struct in_addr sin_addr
    if (inet_aton("127.0.0.1", &addr.sin_addr) == 0) {
        perror("inet_aton() fails");
        exit(EXIT_FAILURE);
    }

    if (connect(fd, (struct sockaddr *)&addr, sizeof(addr)) == -1) {
        perror("connect() fails");
        exit(EXIT_FAILURE);
    }

    char buf[1024] = { 0 }; // buf 가 충분하면 OV 안남.
    fgets(buf, sizeof(buf), stdin);

    uint16_t msg_len = strlen(buf);
    uint16_t msg_len_send = htons(msg_len);
    send_bytes(fd, (char *) &msg_len_send, 2); //msglen : 0~ (2^32)-1
    send_bytes(fd,              buf, msg_len);

    printf("send msg_len : %d\n", msg_len);

    //while(1);

    exit(EXIT_SUCCESS);
}