#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#include "match.h"

/* TODO: Add more functions if needed */

int conn_fd = -1;

int send_bytes(char * buf, int bytes) {
	ssize_t current_len = 0, temp_len = 0;
	while(current_len < bytes){
		temp_len = send(conn_fd, buf+current_len, bytes-current_len, 0);
		if(temp_len < 0) {
			perror("send_bytes");
			exit(EXIT_FAILURE);
		}
		current_len += temp_len;
	}
	return 0;
}

int recv_bytes(char * buf, int bytes) {
	ssize_t current_len = 0, temp_len = 0;
	while(current_len < bytes){
		temp_len = recv(conn_fd, buf+current_len, bytes-current_len, 0);
		if(temp_len == 0) {
			return 1; // EOF
		}
		if(temp_len < 0) {
			perror("recv_bytes");
			exit(EXIT_FAILURE);
		}
		current_len += temp_len;
	}
	return 0;
}


int main (int argc, char ** argv)
{
	char * hostname = NULL ;
	int port = 0 ;
	char * username = NULL ;
	int opt ;
	while ((opt = getopt(argc, argv, "h:p:u:")) != -1) {
		switch (opt) {
			case 'h':
				hostname = optarg ;
				break ;
			case 'p':
				port = atoi(optarg) ;
				break ;
			case 'u':
				username = optarg ;
				break ;
			default:
				fprintf(stderr, "Invalid arguments\n") ;
				exit(EXIT_FAILURE) ;
		}
	}
	if (hostname == NULL || port == 0 || username == NULL) {
		fprintf(stderr, "Invalid arguments\n") ;
		exit(EXIT_FAILURE) ;
	}

	//(1 ≤ n ≤ MAX_NAME (31)). => 0은 전송 무시하고 32B까지 받음
	if(strlen(username) < 1 || strlen(username) > 31 ){
		printf("username : (1 ≤ n ≤ MAX_NAME (31))\n");
		exit(EXIT_FAILURE);
	}

	/*TO-DO(8): connection */
	conn_fd = socket(AF_INET, SOCK_STREAM, 0);
	if(conn_fd == -1) { exit(EXIT_FAILURE); }

	struct sockaddr_in addr;
	addr.sin_family = AF_INET;
	addr.sin_port = htons(port);

	if(inet_aton(hostname, &addr.sin_addr) == 0) {
		fprintf(stderr, "inet_aton() fails");
		exit(EXIT_FAILURE);
	}
	if(connect(conn_fd, (struct sockaddr *) &addr, sizeof(addr))){
		perror("connect");
		exit(EXIT_FAILURE);
	}

	printf("connected! conn_fd : %d\n", conn_fd);

	/*TO-DO(9): send the username to the server */

	// username 길이 보내지말고 자체적으로 전송 : 32B 최대
	send_bytes(username, 32);

	/*TO-DO(10): receive the team message from the server, and print it */
	// team msg 길이 범위가 4B : uint32_t 이다.
	uint32_t recv_msg_len;
	recv_bytes((char *) &recv_msg_len, 4);

	uint32_t msg_len = ntohl(recv_msg_len);
	char * team_message = calloc(msg_len,1);
	recv_bytes(team_message, msg_len);

	printf("%s\n", team_message);

	exit(EXIT_SUCCESS) ;
}
