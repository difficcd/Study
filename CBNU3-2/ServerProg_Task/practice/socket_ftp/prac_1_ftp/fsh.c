
#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>
#include <endian.h>
#include <signal.h>

#define BACKLOG 2

int conn_fd;

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


    // filedir content_len, content 받기
    uint64_t recv_content_len;
    recv_bytes(conn_fd, (char *) &recv_content_len, 8);

    uint64_t content_len = be64toh(recv_content_len);
    char * content = calloc(content_len,1);
    recv_bytes(conn_fd, content, content_len);

    // display 해보기
    uint64_t i = 0;
    printf("\n");
    while(i < content_len){
        if(content[i] == '\0') printf("\n");
        if(content[i] == '.' && i+1 < content_len && 
           (content[i+1] == '\0' || content[i+1] == '.')) {
            if (content[i+1] == '.') i+=3;
            else i+=2; 
            continue;
        }
        else printf("%c", content[i]);
        i++;
    }
}

// get src dst -> src 가 다운로드 대상 파일, dst 가 가져올 그릇 파일
void get_req(int conn_fd, char * src, char *dst) {
    uint8_t command = 1; // list 0 get 1 put 2
    send_bytes(conn_fd, (char *) &command, 1);

    // src 부터 보내고 dst 보냄
    uint16_t src_len = strlen(src);
    uint16_t send_src_len = htons(src_len);
    send_bytes(conn_fd, (char *) &send_src_len, 2);
    send_bytes(conn_fd, src, src_len);

    uint16_t dst_len = strlen(dst);
    uint16_t send_dst_len = htons(dst_len);
    send_bytes(conn_fd, (char *) &send_dst_len, 2);
    send_bytes(conn_fd, dst, dst_len);

}

// put src dst -> src 의 파일 내용 추출해서 서버의 dst 파일로 보내기
void put_req(int conn_fd, char * src, char *dst) {

}

void signal_handler(int signum){
    uint8_t end = 1 ;
    send_bytes(conn_fd, (char *) &end, 1);

    exit(EXIT_SUCCESS);
}

int main(int argc, char ** argv) {

    if (argc != 4 && argc != 5) {
        printf("Usage: ./fsh <port> <command> [args]\n");
        exit(EXIT_FAILURE);
    }
    signal(SIGINT,  signal_handler);
    signal(SIGTERM, signal_handler);

    conn_fd = socket(AF_INET, SOCK_STREAM, 0);
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

    // server가 먼저 들어갔는지 아닌지 알려줘야 함. flag send
    uint8_t flag ;
    recv_bytes(conn_fd, (char *) &flag, 1);
    printf("flag : %d\n", flag);
    if(flag == 1) {
        printf("max user, connection refused!\n");
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

    // client 는 끝났을 때 end 전송해서 서버가 자원정리하게 함. (sighand)
    // persistent 하게 해두고 디버깅 (3개만 들어오는지, 죽이고나면 다시 받는지)
    while(1);
    
    close(conn_fd);

}