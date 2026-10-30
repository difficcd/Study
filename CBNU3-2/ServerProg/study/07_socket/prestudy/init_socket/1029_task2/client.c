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
	while(current_len < len){
		sent_bytes = send(fd, p+current_len, len-current_len, 0);
		if(sent_bytes == -1){
			perror("string send: failed");
			return 1;
		}
		current_len += sent_bytes;
	}
	return 0;
}


int main (int argc, char ** argv) 
{
	if(argc != 6) {
		printf("usage: ./client -a <ip> -p <port-number> <filename>\n");
		return 1;
	}

	// ==== option parsing -a(ip) -p(port_num) ==== //
	int opt; char *ip; int port;

	do {
		opt = getopt(argc, argv, "a:p:");
		if (opt == 'a') 
			ip = optarg; 
		else if (opt == 'p') 
			port = atoi(optarg);

	} while (opt != -1);
	char *filename = argv[optind];  // optind == opt idx

	// ==== file scan ==== //
	FILE *fp = fopen(filename, "rb");
	if(fp == NULL){
		perror("file open error: ");
		return 1;
	}
	fseek(fp, 0, SEEK_END);    // END 로 이동 (offset=0) => size도출!
	int file_size = ftell(fp); // 현재 fp 가 몇번째 byte 가리키나..
	fseek(fp, 0, SEEK_SET);    // 다시 맨 처음으로 이동 시켜두기.

	
	// ==== client : connection establish ==== //
	int fd = socket(AF_INET, SOCK_STREAM, 0) ;

	struct sockaddr_in srv_addr ;
	srv_addr.sin_family = AF_INET ;
	srv_addr.sin_port = htons(port) ;

	if (inet_aton(ip , &(srv_addr.sin_addr)) == 0) {
		fprintf(stderr, "inet_aton(127.0.0.1, addr.sin_addr) returns zero\n") ;
		exit(EXIT_FAILURE) ;
	}
	if (connect(fd, (struct sockaddr *) &srv_addr, sizeof(srv_addr)) == -1) {
		perror("connect() fails") ;
		exit(EXIT_FAILURE) ;
	}


	// ==== client -> server file transmit ==== // 
	uint32_t name_len = strlen(filename);
	uint32_t n1 = htonl(name_len), n2 = htonl(file_size);

	send_all(fd, (char *)&n1, 4);
	send_all(fd, (char *)&n2, 4);
	send_all(fd, filename, name_len);

	char buf[4096]; 
	int n;
	do {
		n = fread(buf, 1, sizeof(buf), fp);
		send_all(fd, buf, n);
	} while(n > 0);
	shutdown(fd, SHUT_WR);                     
	fclose(fp);


	// server resopnse print 
	char res[256]; 
	int got = 0, r;
	do {
		r = recv(fd, res + got, sizeof(res)-1-got, 0);
		got += r;
	}while (r > 0);
	res[got] = '\0';
	printf("server: %s\n", res);

	close(fd) ;

	return EXIT_SUCCESS ;
}
