#include <ncurses.h>
#include <pthread.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <stdatomic.h>
#include <netinet/in.h>
#include <netinet/tcp.h>
#include <arpa/inet.h>
#include <sys/time.h>

int conn = -1 ;
int iter = 0 ;

ssize_t send_bytes (int conn, char *buf, size_t len) 
{
    int current_len = 0, sent_bytes = 0;
	while(current_len < len){
		sent_bytes = send(conn, buf+current_len, len-current_len, 0);
		if(sent_bytes <= 0){
			perror("string send: failed");
			return -1;
		}
		current_len += sent_bytes;
	}
	return current_len;
}


void * recipent_thread_runner (void * arg)
{
	if (conn == -1) return NULL ;

	int conn_fail_or_quit = 0 ;
	while (conn_fail_or_quit == 0) {
		char buf[1024] ;
		ssize_t m ;
		memset(buf, 0, 1024) ;
		m = recv(conn, buf, 1024, 0) ;
	}
	return NULL ;
}

void * user_thread_runner (void * arg)
{
	struct timeval begin ;
	struct timeval end ;
	/*TODO*/

	gettimeofday(&begin, NULL);
	
	// 1B send 를 10^6 회 하도록 .. 
	
	int i = 0;
	char buf[512];
	while(i < 100) {
		// send_bytes(conn, word, 1);
		if(send(conn, buf, 1, 0) < 0) {
			exit(EXIT_FAILURE) ;
		} 
		i++;
	}
	gettimeofday(&end, NULL);

	printf("%ld\n", end.tv_usec - begin.tv_usec);
	
	while(1) {}

	return NULL ;
}


int main (int argc, char ** argv)
{
	char username[16] = "hongshin" ;
	char password[16] = "hongshin" ;
	char hostname[32] = "127.0.0.1" ;
	int port = 0 ;

	int opt ;
	while ((opt = getopt(argc, argv, "u:s:h:p:i:")) != -1) {
		switch (opt) {
			case 'u':
				strncpy(username, optarg, 16) ;
				break ;
			case 's':
				strncpy(password, optarg, 16) ;
				break ;
			case 'h':
				strncpy(hostname, optarg, 32) ;
				break ;
			case 'p':
				port = atoi(optarg) ;
				break ;
			case 'i':
				iter = atoi(optarg) ;
				break ;

			default:
				fprintf(stderr, "Invalid arguments\n") ;
				exit(EXIT_FAILURE) ;
		}		
	}
	if (username[0] == '\0' || password[0] == '\0' || hostname[0] == '\0' || port == 0) {
		fprintf(stderr, "Invalid arguments\n") ;
		exit(EXIT_FAILURE) ;
	}

	conn = socket(AF_INET, SOCK_STREAM, 0) ;
	if (conn < 0) {
		fprintf(stderr, "socket() fails.\n") ;
		exit(EXIT_FAILURE) ;
	}

	if (getenv("NODELAY")){
		int flag = 1;
		if(setsockopt(conn, IPPROTO_TCP, TCP_NODELAY, &flag, sizeof(flag)) == -1) {
			fprintf(stderr, "setsockopt() fails" ) ;
			exit(EXIT_FAILURE);
		}
	}

	struct sockaddr_in srv_addr ;
	srv_addr.sin_family = AF_INET ;
	srv_addr.sin_port = htons(port) ;
	if (inet_aton(hostname, &(srv_addr.sin_addr)) == 0) {
		fprintf(stderr, "inet_aton(127.0.0.1, addr.sin_addr) returns zero\n") ;
		exit(EXIT_FAILURE) ;
	}
	if (connect(conn, (struct sockaddr *) &srv_addr, sizeof(srv_addr)) == -1) {
		fprintf(stderr, "connect() fails\n") ;
		exit(EXIT_FAILURE) ;
	}

	pthread_t recipent ;
	pthread_t sender ;

	pthread_create(&recipent, NULL, recipent_thread_runner, NULL) ;
	pthread_create(&sender, NULL, user_thread_runner, NULL) ;

	pthread_join(recipent, NULL) ;
	pthread_join(sender, NULL) ;


	exit(EXIT_SUCCESS) ;
}
