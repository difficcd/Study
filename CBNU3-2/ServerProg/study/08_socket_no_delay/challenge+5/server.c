#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>
#include <endian.h>
#include <sys/stat.h>
#include <arpa/inet.h>
#include <time.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <signal.h>

#include "snake.h"

#ifndef MAXUSER
#define MAXUSER 4
#endif

int listen_fd;
int num_player = 2;

uint8_t user_count;
uint8_t color[MAXUSER] = {0, 1, 2, 3}; 


// send helper
ssize_t send_bytes (int conn, char *buf, size_t len) 
{
    int current_len = 0, sent_bytes = 0;
	while(current_len < len){
		sent_bytes = send(conn, buf+current_len, len-current_len, 0);
		// printf("debug sent bytes : %d\n", sent_bytes);
		if(sent_bytes < 0){
			// perror("string send: failed");
			return -1;
		}
		current_len += sent_bytes;
	}
	return current_len;
}

// recv helper 
ssize_t recv_bytes (int conn, char *buf, size_t len) 
{
    int current_len = 0, received_bytes = 0;
	while(current_len < len){
		received_bytes = recv(conn, buf+current_len, len-current_len, 0) ;

		if(received_bytes == -1){
			// perror("string receive: failed");
			return -1;
		}
		if(received_bytes == 0) {
            return 0;
		}  // 무한 루프 방지 
		current_len += received_bytes;
	} // recv(conn, res, len, MSG_WAITALL); 
	return current_len;
}    


int broadcast (char * str, size_t len) // broadcast(buf, strlen(buf)) ;
{
    // broadcast upstream/signin message
    // signin message : "[Server] %s signs in.\n" (buf[512])

   
}


void * client_service_runner (void * arg) 
{
    int conn_fd = *((int *)arg) ;
    free(arg) ;

    // (1) color info send 
    send(conn_fd, (char *)&color[user_count], 1, 0);
    user_count ++;

    // if(num_player == user_count)
    //send(conn_fd, , 1, 0);
    user_count ++;

    // (2) user, color broadcast

    /*TO-DO(3): receive the signin message */
    usleep(10000000);

}

void sig_handler (int signum) 
{
    /*TO-DO(7): Close all connections and terminate the program when SIGINT is called.*/
    close(listen_fd);

    exit(0);

}


int main (int argc, char ** argv)
{
    char * host = strdup("127.0.0.1") ;
    int port = 0 ;

    user_count = 0;

    if(argc != 7) {
        printf("Usage: ./server -p <portnum> -h <host> -n <num_player>\n");
        return 1;
    }

    signal(SIGINT,  sig_handler);
    signal(SIGTERM, sig_handler);
    signal(SIGPIPE, SIG_IGN); // exit 사용자로 인해 서버 죽는 것 방지..


    int opt ;
    while ((opt = getopt(argc, argv, "p:h:n:")) != -1) {
        switch (opt) {
            case 'p':
                port = atoi(optarg) ;
                break;
            case 'h':
                free(host) ;
                host = strdup(optarg) ;
                break;
            case 'n':
                num_player = atoi(optarg) ;
                break;
            default:
                fprintf(stderr, "Unknown option: %s\n", optarg) ;
                exit(EXIT_FAILURE) ;
        }
    }

    /*TO-DO(8): bind and listen*/
    listen_fd = socket(AF_INET, SOCK_STREAM, 0);

    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_port = htons(port);

    if (inet_aton("127.0.0.1", &(addr.sin_addr)) == 0) {
		fprintf(stderr, "inet_aton(127.0.0.1, addr.sin_addr) returns zero\n") ;
		exit(EXIT_FAILURE) ;
	}
	if (bind(listen_fd, (struct sockaddr *) &addr, sizeof(addr)) == -1) {
		perror("bind() fails") ;
		exit(EXIT_FAILURE) ;
	}
	if (listen(listen_fd, num_player) == -1) {
		perror("listen(fd,16) fails") ;
		exit(EXIT_FAILURE) ;
	}
    
    do {
        int conn_fd = accept(listen_fd, NULL, NULL);
        if (conn_fd == -1) {
            perror("accept");
            exit(EXIT_FAILURE);
        }

        pthread_t client_service_thread ;
        int *arg = malloc(sizeof(int));
        *arg = conn_fd;
        pthread_create(&client_service_thread, NULL, client_service_runner, (void *)arg) ;
    } while (1) ;


    close(listen_fd);

    exit(EXIT_SUCCESS) ;
}