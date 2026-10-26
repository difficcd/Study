#include <unistd.h> 
#include <stdio.h> 
#include <sys/socket.h> 
#include <stdlib.h> 
#include <netinet/in.h> 
#include <string.h> 

char * msg = 0x0 ;
int msg_size = 0 ;

void
receive_one (int conn)
{
	char buf[1024] ;
	char * data  = 0x0 ;
	int size = 0 ;
	int s = 0 ;

	while ( (s = recv(conn, buf, 1024, 0)) > 0 ) {
		if (data == 0x0) {
			data = malloc(s) ;
			memcpy(data, buf, s) ;
			size = s ;
		}
		else {
			data = realloc(data, size + s + 1) ;
			memcpy(data + size, buf, s) ;
			size += s ;
		}

	}
	printf(">%s\n", data) ;
	msg = data ;
	msg_size = size ;
}

void
sendout_one (int conn)
{
	char * data = msg ;
	int size = msg_size ;
	int s = 0 ;

	while (size > 0 && (s = send(conn, data, size, 0)) > 0) {
		data += s ;
		size -= s ;
	}
	shutdown(conn, SHUT_WR) ;
	if (msg != NULL) {
		free(msg) ;
	}
}

int 
main(int argc, char const *argv[]) 
{ 
	int listen_fd, new_socket ; 
	struct sockaddr_in address; 
	int opt = 1; 
	int addrlen = sizeof(address); 

	char buffer[1024] = {0}; 

	if ((listen_fd = socket(AF_INET /*IPv4*/, SOCK_STREAM /*TCP*/, 0 /*IP*/)) == 0) {
		perror("socket failed : "); 
		exit(EXIT_FAILURE); 
	}

	memset(&address, '0', sizeof(address)); 
	address.sin_family = AF_INET; 
	address.sin_addr.s_addr = INADDR_ANY /* the localhost*/ ; 
	address.sin_port = htons(8080); 
	if (bind(listen_fd, (struct sockaddr *)&address, sizeof(address)) < 0) {
		perror("bind failed : "); 
		exit(EXIT_FAILURE); 
	} 

	if (listen(listen_fd, 16 /* the size of waiting queue*/) < 0) { 
		perror("listen failed : "); 
		exit(EXIT_FAILURE); 
	} 

	new_socket = accept(listen_fd, (struct sockaddr *) &address, (socklen_t*)&addrlen) ;
	if (new_socket < 0) {
		perror("accept"); 
		exit(EXIT_FAILURE); 
	} 

	receive_one(new_socket) ;
	sendout_one(new_socket) ;

	exit(EXIT_SUCCESS) ;
} 
