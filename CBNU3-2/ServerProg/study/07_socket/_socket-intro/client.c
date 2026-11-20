#include <stdio.h> 
#include <sys/socket.h> 
#include <stdlib.h> 
#include <netinet/in.h> 
#include <arpa/inet.h>
#include <string.h> 

int sendout_one (int conn, char * data, int len) {
	int s = 0 ;
	while (len > 0 && (s = send(conn, data, len, 0)) > 0) {
		data += s ;
		len -= s ;
	}
	shutdown(conn, SHUT_WR) ;
	return 0 ;
}

int receive_one (int conn, char **data) {
	char buf[1024] ;
	*data = 0x0 ;
	int size = 0 ;
	int s = 0 ;
	while ( (s = recv(conn, buf, 1024, 0)) > 0 ) {
		if (*data == 0x0) {
			*data = malloc(s) ;
			memcpy(*data, buf, s) ;
			size = s ;
		}
		else {
			*data = realloc(data, size + s) ;
			memcpy(*data + size, buf, s) ;
			size += s ;
		}

	}
	return size ;
}

int 
main (int argc, char const *argv[]) 
{ 
	struct sockaddr_in serv_addr; 
	int sock_fd ;
	int s, len ;
	char buffer[1024] = {0}; 
	char * data ;
	
	sock_fd = socket(AF_INET, SOCK_STREAM, 0) ;
	if (sock_fd <= 0) {
		perror("socket failed : ") ;
		exit(EXIT_FAILURE) ;
	} 

	memset(&serv_addr, '\0', sizeof(serv_addr)); 
	serv_addr.sin_family = AF_INET; 
	serv_addr.sin_port = htons(8080); 
	if (inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr) <= 0) {
		perror("inet_pton failed : ") ; 
		exit(EXIT_FAILURE) ;
	} 

	if (connect(sock_fd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0) {
		perror("connect failed : ") ;
		exit(EXIT_FAILURE) ;
	}

	scanf("%1023[^\n]%*c", buffer) ;
	
	data = buffer ;
	len = strlen(buffer) ;
	sendout_one(sock_fd, data, len) ;
	
	data = 0x0 ;
	int size ;
	size = receive_one(sock_fd, &data) ;

	for (int i = 0 ; i < size ; i++) {
		printf("%c", data[i]) ;
	}
	printf("\n") ;

	return EXIT_SUCCESS ;
} 
