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
		/*
			int sent_bytes ;
			sent_bytes = send(fd, message, 1024, 0) ;
			printf("send typed string to server : %d\n", sent_bytes) ;
		*/
	//

	// send typed string (1) : variable length

	// int buf_len = 1024;
	// char * message = malloc(buf_len);
	printf("type string : ");

	char * message = NULL; 
	size_t len = 0; 
	ssize_t scan_res = getline(&message, &len, stdin);
	if(scan_res == -1){
		perror("getline: failed");
		return 1;
	}
	
	//scanf("%s", message);


	int total_len = strlen(message);
	int current_len = 0, sent_bytes = 0;
	while(current_len < total_len){
		sent_bytes = send(fd, message+current_len, total_len-current_len, 0);
		if(sent_bytes == -1){
			perror("string send: failed");
			return 1;
		}
		current_len += sent_bytes;
	}
	shutdown(fd, SHUT_WR); 
	// server 의 recv blocking shutdown, client 는 res 받아서 정상종료 가능
	// cond var 에서 signal 보내는 것과 비슷한 느낌 


	// receive reverse string (2) 
		/*
			char rev_buf[512];
			int bytes_recv ;
			bytes_recv = recv(fd, rev_buf, 512, 0) ;
			printf("recv from server: reverse echo => \"%s\" (%d)\n", rev_buf, bytes_recv) ;
		*/
	//

	// receive reverse string (2) : variable length

	char * recv_buf = malloc(total_len + 1);  // recv 라서 \0 추가 필요 
	current_len = 0;
	int recv_bytes = 0;
	while(current_len < total_len){
		recv_bytes = recv(fd, recv_buf+current_len, total_len-current_len, 0) ;
		if(recv_bytes == -1){
			perror("string receive: failed");
			return 1;
		}
		if(recv_bytes == 0) break; 

		current_len += recv_bytes;
	} // recv(fd, res, len, MSG_WAITALL); 

	recv_buf[current_len] = '\0';  


	printf("recv from server: reverse echo \n => \"%s\" (%d)\n", recv_buf, current_len) ;

	free(message);
	free(recv_buf);
	close(fd) ;

	return EXIT_SUCCESS ;
}
