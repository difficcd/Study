#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

int send_all(int fd, char *p, int len)   { 
	
	/* Task1 send 반복문 */ 
	int current_len = 0, sent_bytes = 0;
	while(current_len < total_len){
		sent_bytes = send(fd, p+current_len, total_len-current_len, 0);
		if(sent_bytes == -1){
			perror("string send: failed");
			return 1;
		}
		current_len += sent_bytes;
	}
	return 0;
}

int recv_exact(int fd, char *p, int len) { 
	/* Task1 클라 recv 반복문 (len 만큼) */

	int current_len = 0, recv_bytes = 0;
	while(current_len < len){
		recv_bytes = recv(fd, p+current_len, len-current_len, 0) ;
		if(recv_bytes == -1){
			perror("string receive: failed");
			return 1;
		}
		if(recv_bytes == 0) break; 

		current_len += recv_bytes;
	} // recv(fd, res, len, MSG_WAITALL); 
	return 0;
}


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
	socklen_t peer_len = sizeof(peer);
	
	int fda ;
	fda = accept(fd, (struct sockaddr *) &peer, &peer_len) ;

	

	close(fda) ;
	close(fd) ;

	return EXIT_SUCCESS ;
}
