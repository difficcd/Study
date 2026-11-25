
#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>

#define BACKLOG 2


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



// list up filedir contents (filedir == file)
void list_req(int conn_fd, char * filepath) {   

    uint8_t command = 0; // list 0 get 1 put 2
    send_bytes(conn_fd, (char *) &command, 1);


    // 파일 경로 길이는 0~2^16-1 수준으로..
    uint16_t path_len = strlen(filepath);
    uint16_t send_path_len = htons(path_len);

    send_bytes(conn_fd, (char *) &send_path_len, 2);
    send_bytes(conn_fd, filepath, path_len);


    // display 해보기

}

// get src dst -> src 가 다운로드 대상 파일, dst 가 가져올 그릇 파일
void get_req(int conn_fd, char * src, char *dst) {

}

// put src dst -> src 의 파일 내용 추출해서 서버의 dst 파일로 보내기
void put_req(int conn_fd, char * src, char *dst) {

}


int main(int argc, char ** argv) {

    if (argc != 4 && argc != 5) {
        printf("Usage: ./fsh <port> <command> [args]\n");
        exit(EXIT_FAILURE);
    }

    int conn_fd = socket(AF_INET, SOCK_STREAM, 0);
    printf("conn_fd: %d\n", conn_fd);

    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_port = htons(atoi(argv[1]));

    // struct in_addr *inp
    if (inet_aton("127.0.0.1", &addr.sin_addr) == 0) {
        fprintf(stderr, "inet_aton() fails");
        close(conn_fd);
        exit(EXIT_FAILURE);
    }
    if (connect(conn_fd, (struct sockaddr *) &addr, sizeof(addr)) == -1){
        perror("connect() fails");
        close(conn_fd);
        exit(EXIT_FAILURE);
    }

    // 요청을 보낼 때는 list 0, get 1, put 2 로 1B로 전송하도록 함
    if(strcmp(argv[2], "list")     == 0) list_req(conn_fd, argv[3]);
    else if(strcmp(argv[2], "get") == 0)  get_req(conn_fd, argv[3], argv[4]);
    else if(strcmp(argv[2], "put") == 0)  put_req(conn_fd, argv[3], argv[4]);
    else {
        printf("invalid command!\n");
        close(conn_fd);
        exit(EXIT_FAILURE);
    }

    close(conn_fd);

}