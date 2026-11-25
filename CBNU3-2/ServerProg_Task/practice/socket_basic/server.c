#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <pthread.h>

#define MAXUSER 1

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

/*
void * conn_thread_routine(void * args){

    int conn_fd = *((int *)args);
    int check_res ;

    while(conn_fd > 0){
        uint32_t recv_len ;
        check_res = check(recv_bytes(conn_fd, (char *) &recv_len, 4),conn_fd);
        if(check_res == 1) break;

        uint32_t msg_len = ntohs(recv_len);
        if(conn_fd > 0) printf("msg_len : %d \n", msg_len);

        char * buf = calloc(msg_len, sizeof(char));
        check_res = check(recv_bytes(conn_fd, buf, msg_len), conn_fd);
        if(check_res == 1) break;

        printf("recv buf: %s\n", buf);
    }

    return NULL;
}*/



int main(int argc, char ** argv) {

    int listen_fd;
    
    if ((listen_fd = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
        perror("socket() fails");
        exit(EXIT_FAILURE);
    }

    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_port = htons(atoi(argv[1]));

    // struct in_addr sin_addr
    if (inet_aton("127.0.0.1", &addr.sin_addr) == 0) {
        perror("inet_aton() fails");
        exit(EXIT_FAILURE);
    }

    if (bind(listen_fd, (struct sockaddr *)&addr, sizeof(addr)) == -1) {
        perror("bind() fails");
        exit(EXIT_FAILURE);
    }

    if (listen(listen_fd, MAXUSER) == -1) {
        perror("listen() fails");
        exit(EXIT_FAILURE);
    }


    struct sockaddr_in peer;
    socklen_t peer_len = sizeof(peer);

    int conn_fd = accept(listen_fd, (struct sockaddr *)&peer, &peer_len);
    if(conn_fd == -1) {
        perror("accept() fails");
        exit(EXIT_FAILURE);
    }

    printf("peer info : peer port: %d, peer len: %u\n", 
                        ntohs(peer.sin_port), peer_len);

    int * pfd = &conn_fd;
    int check_res;
    
    uint16_t recv_len ;
    check_res = check(recv_bytes(conn_fd, (char *) &recv_len, 4),conn_fd);
    if(check_res == 1) return 1;

    uint16_t msg_len = ntohs(recv_len);
    if(conn_fd > 0) printf("msg_len : %d \n", msg_len);

    char * buf = calloc(msg_len, sizeof(char));
    check_res = check(recv_bytes(conn_fd, buf, msg_len), conn_fd);
    if(check_res == 1) return 1;

    printf("recv buf: %s", buf);

    close(listen_fd);

}