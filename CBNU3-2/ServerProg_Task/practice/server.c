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

void * conn_thread_routine(void * args){

    int conn_fd = *((int *)args);

    while(1){

        uint32_t msg_len_recv ;
        recv_bytes(conn_fd, (char *) &msg_len_recv, 4);

        uint32_t msg_len = ntohs(msg_len_recv);
        char * buf = calloc(msg_len, sizeof(char));
        int res = recv_bytes(conn_fd, buf, msg_len_recv);
        if(res > 0) {
            printf("res bytes : %d \n buf: %s\n", res, buf);
        }

        if(res == 0) {close(conn_fd); return NULL;}
    }
}



int main() {

    int listen_fd;
    
    if ((listen_fd = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
        perror("socket() fails");
        exit(EXIT_FAILURE);
    }
    printf("listen_fd: %d\n", listen_fd);

    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_port = htons(12189);

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

    do {
        pthread_t conn_thread;
        int conn_fd = accept(listen_fd, NULL, NULL);
        if(conn_fd == -1) {
            perror("accept() fails");
            exit(EXIT_FAILURE);
        }
        else printf("connected : %d!\n", conn_fd);

        int * pfd = &conn_fd;
        pthread_create(&conn_thread, NULL, conn_thread_routine, (void *)pfd);
        // join 할 필요가 있나? 
        // persistant TCP 에 자기 몫 다 하고 fd끊기면 close해주면되는거아님?

    } while(1);

    close(listen_fd);

}