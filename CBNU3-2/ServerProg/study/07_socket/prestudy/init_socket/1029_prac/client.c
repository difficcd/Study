#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

int main (int argc, char ** argv) 
{
	if(argc != 3) {
		printf("usage: ./oneclient <ip> <port-number>\n");
		return 1;
	}

	int fd = socket(AF_INET, SOCK_STREAM, 0) ;

	struct sockaddr_in srv_addr ;
	srv_addr.sin_family = AF_INET ;
	srv_addr.sin_port = htons(atoi(argv[2])) ;

	if (inet_aton(argv[1], &(srv_addr.sin_addr)) == 0) {
		fprintf(stderr, "inet_aton(127.0.0.1, addr.sin_addr) returns zero\n") ;
		exit(EXIT_FAILURE) ;
	}
	if (connect(fd, (struct sockaddr *) &srv_addr, sizeof(srv_addr)) == -1) {
		perror("connect() fails") ;
		exit(EXIT_FAILURE) ;
	}


	// send typed string (1)
	char * message = malloc(sizeof(char) * 1024);
	printf("type string : ");
	scanf("%s", message);

	int sent_bytes ;
	sent_bytes = send(fd, message, 1024, 0) ;
	printf("send typed string to server : %d\n", sent_bytes) ;


	// receive reverse string (2) 
	char rev_buf[512];
	int bytes_recv ;
	bytes_recv = recv(fd, rev_buf, 512, 0) ;
	printf("recv from server: reverse echo => \"%s\" (%d)\n", rev_buf, bytes_recv) ;

	


	close(fd) ;

	return EXIT_SUCCESS ;
}
