#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <pthread.h>


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



void * recv_msg(void * args){
    while(1){

    }
}

void * send_msg(void * args){
    while(1){

    }
}


int main() {

    int fd;
    
    if ((fd = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
        perror("socket() fails");
        exit(EXIT_FAILURE);
    }
    printf("fd: %d\n", fd);

    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_port = htons(12180);

    // struct in_addr sin_addr
    if (inet_aton("127.0.0.1", &addr.sin_addr) == 0) {
        perror("inet_aton() fails");
        exit(EXIT_FAILURE);
    }

    if (connect(fd, (struct sockaddr *)&addr, sizeof(addr)) == -1) {
        perror("connect() fails");
        exit(EXIT_FAILURE);
    }

    char buf[16] = "hellow world!\0";
    send_bytes(fd, buf, 16);

    pthread_create(&recv_thread, NULL, recv_msg, NULL);
    pthread_create(&send_thread, NULL, send_msg, NULL);
    pthread_join(recv_thread, NULL);
    pthread_join(send_thread, NULL);

}