
// string 보내면 거꾸로 보내는 echo server

#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define QUEUE_LEN 16

int main()
{
    int fd = socket(AF_INET, SOCK_STREAM, 0);
    printf("socket fd : %d\n", fd);

    // ip :  https://linux.die.net/man/7/ip
    struct sockaddr_in addr; 
    addr.sin_family = AF_INET;         // ipv4
    addr.sin_port = htons(12100);      // host -> network(byte order)
               

    int inet = inet_aton("127.0.0.1", &addr.sin_addr);
    if(inet == 0) return 1;

    int bindtest;
    bindtest = bind(fd, (struct sockaddr *)&addr, sizeof(addr));
    if(bindtest) return 1; // perror()

    int listen_res;
    listen_res = listen(fd, QUEUE_LEN);
    // int backlog : specifies the queue length 
    // for completely established sockets waiting to be accepted

    struct sockaddr_in peer;
    socklen_t peer_len = sizeof(peer);

    int accept_res;
    accept_res = accept(fd, (struct sockaddr *)&peer, &peer_len);
    if (accept_res < 0) 
        perror("accept failed");
    
    // 이 시점부터 server가 대기를 해서 ctrl+c 로 해야함.
    // pear 로 받아야 return 된 fd를 받을 수 있음.
    
    int recv_res;
    /*
        char buf[512] = {0}; 
        recv_res = recv(accept_res, buf, 512, 0);
        if (recv_res < 0) 
            perror("recv failed");
        printf("res : %s\n", buf);
    */

    char str[512] = {0}; 
    recv_res = recv(accept_res, str, 512, 0);
    if (recv_res < 0) 
        perror("recv failed");
    printf("input res : %s\n", str);
    
    
    do {
        char buf[1024];
        int bytes_recv;
        int str_len = strlen(tmp);

        if(str_len + current_len > buf_len) {
            realloc(str, (str_len + current_len)*2);
        }

        if(str_send_res == -1)
            perror("string send: failed");

    } while(bytes_recv > 0);



    close(fd);
}

// man <함수이름> 으로 cli 에서 확인

// type : SOCK_STREAM (TCP)
// protocol : default 

// dom, type 모두 int 임.




