#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

int main () 
{

	int fd = socket(AF_INET, SOCK_STREAM, 0) ;

	struct sockaddr_in addr ;
	addr.sin_family = AF_INET ;
	addr.sin_port = htons(19999) ;

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
	// 통신용 소켓 fda 는 필수이나 peer 정보는 NULL로 받아도 되긴 함
	// 접속한 client 의 정보를 알기 위한 목적일 뿐

	char buf[512] ;
	int bytes_recv ;
	bytes_recv = recv(fda, buf, 512, 0) ;
	printf("\"%s\" (%d)\n", buf, bytes_recv) ;

	close(fda) ;
	close(fd) ;

	return EXIT_SUCCESS ;
}
