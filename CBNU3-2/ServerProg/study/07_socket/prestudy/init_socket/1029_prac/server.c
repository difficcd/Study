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
	if(argc != 2) {
		printf("usage: ./revecho <port-number>\n");
		return 1;
	}

	int fd = socket(AF_INET, SOCK_STREAM, 0) ;

	struct sockaddr_in addr ;
	addr.sin_family = AF_INET ;
	addr.sin_port = htons(atoi(argv[1])) ;

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


	// receive typed string (1) 
	char buf[512] ;
	int bytes_recv ;
	bytes_recv = recv(fda, buf, 512, 0) ;
	printf("\"%s\" (%d)\n", buf, bytes_recv) ;

	// reverse logic
	buf[bytes_recv] = '\0';
	int len = strlen(buf);
	char reverse[512];
	int i=0;
	printf("input len: %d\n", len);

	while (i < len && i < 511) { 
		reverse[i] = buf[len - (1+i)];
		i++;
	}
	reverse[i] = '\0';

	// reverse string : send to client (2)
	int sent_bytes ;
	sent_bytes = send(fda, reverse, 512, 0);
	printf("send reverse string (%d)\n", sent_bytes) ;



	close(fda) ;
	close(fd) ;

	return EXIT_SUCCESS ;
}
