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

	struct sockaddr_in addr ;
	addr.sin_family = AF_INET ;
	addr.sin_port = htons(12100) ;

	if (inet_aton("127.0.0.1", &(addr.sin_addr)) == 0) {
		fprintf(stderr, "inet_aton(127.0.0.1, addr.sin_addr) returns zero\n") ;
		exit(EXIT_FAILURE) ;
	}
	if (bind(fd, (struct sockaddr *) &addr, sizeof(addr)) == -1) {
		perror("bind() fails") ;
		exit(EXIT_FAILURE) ;
	}
	if (listen(fd, 16) == -1) {
		perror("listen(fd,16) fails") ;
		exit(EXIT_FAILURE) ;
	}


	struct sockaddr_in peer ;
	socklen_t peer_len ;
	int fda ;
	fda = accept(fd, (struct sockaddr *) &peer, &peer_len) ;

	char buf[512] ;
	int bytes_recv ;
	bytes_recv = recv(fda, buf, 512, 0) ;
	printf("\"%s\" (%d)\n", buf, bytes_recv) ;


	// server->client
	char * message;
	scanf("%s", message);

	int sent_bytes ;
	sent_bytes = send(fd, message, strlen(message) + 1, 0) ;
	printf("send(fd, message, strlen(message) +1 , 0)=%d\n", sent_bytes) ;


	close(fda) ;
	close(fd) ;

	return EXIT_SUCCESS ;
}
