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
	socklen_t peer_len = sizeof(peer);
	
	int fda ;
	fda = accept(fd, (struct sockaddr *) &peer, &peer_len) ;

	// receive typed string (1) 
		/*
		char buf[512] ;
		int bytes_recv = recv(fda, buf, 512, 0) ;
		printf("\"%s\" (%d)\n", buf, bytes_recv) ;
		*/
	//

	// receive typed string (1) : variable length
	int buf_len = 1024;
	int part_len = 512;

	char * buf = malloc(buf_len);
	char temp[part_len];
	int total_len = 0;

	int recv_bytes;

	do {
		recv_bytes = recv(fda, temp, part_len, 0);
		if(recv_bytes == -1){
            perror("string send: failed");
			return 1;
		}

		if( recv_bytes + total_len > buf_len) {
			buf_len = (recv_bytes + total_len) * 2;
			buf = realloc(buf, buf_len);
		}

		memcpy(buf + total_len, temp, recv_bytes);   
		
		total_len += recv_bytes;

	} while(recv_bytes != 0);


	// reverse logic (new buf => swap)
	char temp_char;
	for (int j=0; j < total_len / 2; j++) { 
		temp_char = buf[j];
		buf[j] = buf[total_len - (1+j)];
		buf[total_len - (1+j)] = temp_char;
	}


	// reverse string : send to client (2)
		/*
		int sent_bytes ;
		sent_bytes = send(fda, reverse, 512, 0);
		printf("send reverse string (%d)\n", sent_bytes) ;
		*/
	//

	// reverse string : send to client (2) : variable length

	int current_len = 0, sent_bytes = 0;
	while(current_len < total_len){
		sent_bytes = send(fda, buf+current_len, total_len-current_len, 0);
		if(sent_bytes == -1){
			perror("string send: failed");
			return 1;
		}
		current_len += sent_bytes;
	}


	free(buf);
	close(fda) ;
	close(fd) ;

	return EXIT_SUCCESS ;
}
