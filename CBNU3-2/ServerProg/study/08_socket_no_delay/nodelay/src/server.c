#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <endian.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/tcp.h>
#include <arpa/inet.h>

ssize_t send_bytes (int conn, char *buf, size_t len) 
{
    int current_len = 0, sent_bytes = 0;
	while(current_len < len){
		sent_bytes = send(conn, buf+current_len, len-current_len, 0);
		// printf("debug sent bytes : %d\n", sent_bytes);
		if(sent_bytes <= 0){
			perror("string send: failed");
			return -1;
		}
		current_len += sent_bytes;
	}
	return current_len;
}


ssize_t recv_bytes (int conn, char *buf, size_t len) 
{
    int current_len = 0, received_bytes = 0;
	while(current_len < len){
		received_bytes = recv(conn, buf+current_len, len-current_len, 0) ;
		if(received_bytes == -1){
			perror("string receive: failed");
			return -1;
		}
		if(received_bytes == 0) {
			return 0;
		} 
		current_len += received_bytes;
	} 
	return current_len;
}    


int main () 
{
	int fd = socket(AF_INET, SOCK_STREAM, 0) ;
	if (fd < 0) {
		fprintf(stderr, "socket() fails" ) ;
		exit(EXIT_FAILURE) ;
	}

	if (getenv("NODELAY")){
		printf("nodelay\n");
		int flag = 1;
		if(setsockopt(fd, IPPROTO_TCP, TCP_NODELAY, &flag, sizeof(flag)) == -1) {
			fprintf(stderr, "setsockopt() fails" ) ;
			exit(EXIT_FAILURE);
		}
	}

	struct sockaddr_in addr ;
	addr.sin_family = AF_INET ;
	addr.sin_port = htons(0) ;
	if (inet_aton("127.0.0.1", &(addr.sin_addr)) == 0) {
		fprintf(stderr, "inet_aton(127.0.0.1, addr.sin_addr) returns zero\n") ;
		exit(EXIT_FAILURE) ;
	}
	if (bind(fd, (struct sockaddr *) &addr, sizeof(addr)) == -1) {
		perror("bind() fails") ;
		exit(EXIT_FAILURE) ;
	}

	socklen_t addrlen ;
	if (getsockname(fd, (struct sockaddr *)&addr, &addrlen) != 0) {
		perror("getsockname() fails") ;
		return EXIT_FAILURE ;
	}
	printf("bound to port %d\n", ntohs(addr.sin_port)) ;
	if (listen(fd, 16) == -1) {
		perror("listen(fd,16) fails") ;
		exit(EXIT_FAILURE) ;
	}

	while (1) {

		struct sockaddr_in peer ;
		socklen_t peer_len ;
		int conn ;
		conn = accept(fd, (struct sockaddr *) &peer, &peer_len) ;
	
		// size_t recv_bytes = 0 ;	
		char buf[4096] ;
		ssize_t len = 0 , file_len = 0 ;
		ssize_t m ;

		do {
			memset(buf, 0, sizeof(buf)) ;

			recv_bytes(conn, buf, 1);
			// m = recv(conn, buf, sizeof(buf), 0) ;
			/* if (m < 0) {
				fprintf(stderr, "recv() does not receive all content.\n") ;
				exit(EXIT_FAILURE) ;
			} */

			send_bytes(conn, buf, 1);
			//while ((m = send(conn, buf, m, 0)) < 0) {
				// fprintf(stderr, "send() does not send all content.\n") ;
				// exit(EXIT_FAILURE) ;
			//}
		} while (1) ;
		close(conn) ;
	}

	close(fd) ;
	return EXIT_SUCCESS ;
}
