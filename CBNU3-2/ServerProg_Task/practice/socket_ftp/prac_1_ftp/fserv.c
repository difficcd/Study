
#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <pthread.h>
#include <dirent.h>
#include <string.h>

#ifndef BACKLOG
#define BACKLOG 2
#endif

#ifndef MAXUSER
#define MAXUSER 8
#endif


pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t wait_empty = PTHREAD_COND_INITIALIZER;

int thread_cnt = 0;


// send helper
void send_bytes(int conn_fd, char * buf, int bytes) {
    ssize_t current_len = 0, temp_bytes = 0;
    while(current_len < bytes){
        temp_bytes = send(conn_fd, buf + current_len, bytes - current_len, 0);
        if(temp_bytes < 0) {
            perror("send_bytes() fails");
            exit(EXIT_FAILURE);
        }
        current_len += temp_bytes;
    }
}

// recv helper
void recv_bytes(int conn_fd, char * buf, int bytes) {
    ssize_t current_len = 0, temp_bytes = 0;
    while(current_len < bytes){
        temp_bytes = recv(conn_fd, buf + current_len, bytes - current_len, 0);
        if(temp_bytes < 0) {
            perror("recv_bytes() fails");
            exit(EXIT_FAILURE);
        }
        current_len += temp_bytes;
    }
}



void list_res(int conn_fd){

    uint16_t recv_path_len;
    recv_bytes(conn_fd, (char *) &recv_path_len, 2);
    uint16_t path_len = ntohs(recv_path_len);

    char * filepath = calloc(path_len, 1);
    recv_bytes(conn_fd, filepath, path_len);
    
    printf("recv filepath : %s\n\n", filepath);

    // file_contents_len
    DIR * dir = opendir(filepath);
    if(dir == NULL){
        perror("opendir() fails");
        return;
    }

    struct dirent * entry;
    size_t content_len = 0;
    while((entry = readdir(dir)) != NULL) {
        content_len += strlen(entry->d_name) + 1 ;
    } closedir(dir);

    printf("file len : %ld\n", content_len);

    // file_contetns_extract
    char * content = calloc(content_len, 1);
    size_t index = 0;
    dir = opendir(filepath);
    if(dir == NULL){
        perror("opendir() fails");
        return;
    }

    while((entry = readdir(dir)) != NULL) {
        size_t len = strlen(entry->d_name);
        memcpy(content + index, entry->d_name, len);
        index += len;
        content[index] = '\0';
        index++;
    } closedir(dir);

    // content 길이랑 내용 보내고 

}

void get_res(int conn_fd){
    //
}

void put_res(int conn_fd){
    //
}


void * client_routine(void * args){
    int conn_fd = *((int *)args);
    
    // req 함수들의 command 받음
    uint8_t command ;
    recv_bytes(conn_fd, (char *) &command, 1);
    printf("command : %d\n", command);

    // command 에 따라 각 로직 실행 
    if      (command == 0) list_res(conn_fd);
    else if (command == 0)  get_res(conn_fd);
    else if (command == 0)  put_res(conn_fd);
    
  
    pthread_mutex_lock(&lock);
    thread_cnt--;
    pthread_cond_broadcast(&wait_empty);
    pthread_mutex_unlock(&lock);
    // 끝까지 가면 thread_cnt 줄이고 wait_empty broadcast
}


int main(int argc, char ** argv) {

    if (argc != 2) {
        printf("Usage: ./fserv <port> \n");
        exit(EXIT_FAILURE);
    }

    int listen_fd = socket(AF_INET, SOCK_STREAM, 0);
    printf("listen_fd: %d\n", listen_fd);

    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_port = htons(atoi(argv[1]));

    // struct in_addr *inp
    if (inet_aton("127.0.0.1", &addr.sin_addr) == 0) {
        fprintf(stderr, "inet_aton() fails");
        exit(EXIT_FAILURE);
    }
    if (bind(listen_fd, (struct sockaddr *)&addr, sizeof(addr)) == -1){
        perror("bind() fails");
        exit(EXIT_FAILURE);
    }
    if (listen(listen_fd, BACKLOG)){
        perror("listen() fails");
        exit(EXIT_FAILURE);
    }

    do {
        pthread_mutex_lock(&lock);
        while(thread_cnt >= MAXUSER){
            pthread_cond_wait(&wait_empty, &lock);
        }
        pthread_mutex_unlock(&lock);
        // MAXUSER 수 이상이면 블로킹, accept 도 안하게 함

        struct sockaddr_in peer;
        socklen_t addrlen = sizeof(peer);

        int conn_fd = accept(listen_fd, (struct sockaddr *) &peer, &addrlen);
        printf("peer port: %d\n", ntohs(peer.sin_port));

        pthread_t routine;
        int * fd = malloc(sizeof(int));
        *fd = conn_fd;
        pthread_create(&routine, NULL, client_routine, (void *) fd);

        pthread_mutex_lock(&lock);
        thread_cnt ++;
        pthread_mutex_unlock(&lock);

        pthread_detach(routine);

    } while(1);

    close(listen_fd);

}