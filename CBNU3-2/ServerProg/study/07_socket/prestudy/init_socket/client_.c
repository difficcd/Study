#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

int main () 
{

	int fd = socket(AF_INET, SOCK_STREAM, 0) ;
#ifdef DEBUG	
	fprintf(stderr, "socket(AF_INET, SOCK_STREAM, 0)=%d\n", fd) ;
#endif


	struct sockaddr_in srv_addr ;
	srv_addr.sin_family = AF_INET ;
	srv_addr.sin_port = htons(12100) ;

	if (inet_aton("127.0.0.1", &(srv_addr.sin_addr)) == 0) {
		fprintf(stderr, "inet_aton(127.0.0.1, addr.sin_addr) returns zero\n") ;
		exit(EXIT_FAILURE) ;
	}
	if (connect(fd, (struct sockaddr *) &srv_addr, sizeof(srv_addr)) == -1) {
		perror("connect() fails") ;
		exit(EXIT_FAILURE) ;
	}

	char message[] = "Hello world" ;
	int sent_bytes ;

	sent_bytes = send(fd, message, strlen(message) + 1, 0) ;
	printf("send(fd, message, strlen(message) +1 , 0)=%d\n", sent_bytes) ;
	shutdown(fd, SHUT_WR);

	// client<-server
	char buf[512] ;
	int recv_bytes ;
	recv_bytes = recv(fd, buf, 512, 0) ;
	printf("\"%s\" (%d)\n", buf, recv_bytes) ;


	close(fd) ;

	return EXIT_SUCCESS ;
}
