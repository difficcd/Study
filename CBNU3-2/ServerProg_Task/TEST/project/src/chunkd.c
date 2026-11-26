#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <signal.h>
#include <pthread.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/stat.h>

#include "chunk.h"

char * dir = NULL ;

/* TODO: Add more functions if needed */


void * client_service_runner (void * arg) 
{
	int conn_fd = *((int *)arg) ;
	free(arg) ;

	/*TO-DO(3): receive a request and the file name from the client */

	/*TO-DO(4): handle OP_SIZE */

	/*TO-DO(5): handle OP_READ */

	/*TO-DO(6): send the response (and the data), and close the connection */

	return NULL ;
}

int main (int argc, char ** argv)
{
	int port = 0 ;
	int opt ;
	while ((opt = getopt(argc, argv, "p:d:")) != -1) {
		switch (opt) {
			case 'p':
				port = atoi(optarg) ;
				break ;
			case 'd':
				dir = optarg ;
				break ;
			default:
				fprintf(stderr, "Invalid arguments\n") ;
				exit(EXIT_FAILURE) ;
		}
	}
	if (port == 0 || dir == NULL) {
		fprintf(stderr, "Invalid arguments\n") ;
		exit(EXIT_FAILURE) ;
	}

	int listen_fd ;
	/*TO-DO(7): bind and listen */

	do {
		/*TO-DO(8): accept a connection, and create a thread for it */

	} while (1) ;

	exit(EXIT_SUCCESS) ;
}
