
// string 보내면 거꾸로 보내는 echo server

#include <stdio.h>
#include <stdlib.h>
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
    addr.sin_port = htons(12100);      // host -> network

    struct sockaddr_in peer; 
    peer.sin_family = AF_INET;         // ipv4
    peer.sin_port = htons(12100);      // host -> network
               
    
    int inet = inet_aton("127.0.0.1", &addr.sin_addr);
    if(inet == 0) return 1;

    int connect_res; // 성공 시 0, 아니면 -1
    connect_res = connect(fd, (struct sockaddr *)&peer , (socklen_t)16);
    if (connect_res != 0) printf("connection error\n");

    /*
        void * buf;
        buf = "hellow world";

        int send_res = send(fd, buf, 512, 0);
        printf("hellow world >> send complete!\n");
    */

    char * str = malloc(1024); // initial buf
    
        int str_send_res = send(fd, str, sizeof(tmp), 0);
        if(str_send_res == -1)
            perror("string send: failed");
        printf("input string >> send complete!\n");
        

    

}

// man <함수이름> 으로 cli 에서 확인

// type : SOCK_STREAM (TCP)
// protocol : default 

// dom, type 모두 int 임.




