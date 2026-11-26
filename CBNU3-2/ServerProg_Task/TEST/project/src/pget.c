#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <pthread.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#include "chunk.h"

#define NUM_WORKERS 4
#define QUEUE_CAP 8

typedef 
	struct {
		/*TODO*/
	}
	task ;

/*TODO: define the data structure of a bounded buffer */

/*TODO: define other global data structures */

circular_queue_t * buf = 0x0;

void circular_queue_init(int capacity, int n){
	buf->capacity = 0;
	buf->front = 0;
	buf->rear = 0;
}


task * get_task () 
{
	/*TODO*/
}

void put_task ()
{
	/*TODO*/
}

void * worker_runner (void * arg) 
{
	/*TODO*/
	// 큐에서 꺼내서 청크를 recv 하는 함수. (get_task사용 + 블로킹 + end flag)

}

/* TODO: Add more functions if needed*/


/*
A file of size S consists of N chunks. The client prints one line per chunk, in increasing order of
chunk numbers, and then the result.

chunk 1/3 done
chunk 2/3 done
chunk 3/3 done
done: 10000 bytes


큰 파일을 나눠서 다운받는걸 display 하라는 것.
서버의 dir 에 있는 file (-f) 내용 큰 걸 out(-o) 에 적어라 이거임
근데 멀티스레딩이 섞인 => 어케함 ;;;;

Do not keep the whole file in memory.
Use a condition variable to print the lines in order. Busy waiting is not allowed.
*/ 


int main (int argc, char ** argv)
{
	char * hostname = NULL ;
	int port = 0 ;
	char * file = NULL ;
	char * output = NULL ;
	int opt ;
	while ((opt = getopt(argc, argv, "h:p:f:o:")) != -1) {
		switch (opt) {
			case 'h':
				hostname = optarg ;
				break ;
			case 'p':
				port = atoi(optarg) ;
				break ;
			case 'f':
				file = optarg ;
				break ;
			case 'o':
				output = optarg ;
				break ;
			default:
				fprintf(stderr, "Invalid arguments\n") ;
				exit(EXIT_FAILURE) ;
		}
	}
	if (hostname == NULL || port == 0 || file == NULL || output == NULL) {
		fprintf(stderr, "Invalid arguments\n") ;
		exit(EXIT_FAILURE) ;
	}

	/*TODO*/
	// Create exactly NUM_WORKERS (4) worker threads

	int conn_fd = socket(AF_INET, SOCK_STREAM, 0);
	
	struct sockaddr_in addr;
	addr.sin_family = AF_INET;
	addr.sin_port = htons(port);
	
	if(inet_aton(hostname, &addr.sin_addr) == 0){
		fprintf(stderr, "inet_aton() fails");
		exit(EXIT_FAILURE);
	}
	if(connect(conn_fd, (struct sockaddr *) &addr, sizeof(addr))){
		perror("connect");
		exit(EXIT_FAILURE);
	}


	// inserts "chunk numbers" into a bounded buffer of capacity QUEUE_CAP (8)
	// workers take them


	// 여기서 청크 번호를 받고, put 해주어야 함.


	pthread_t workers[NUM_WORKERS];
	for(int i=0; i<NUM_WORKERS, i++){
		pthread_create(&workers[i], NULL, worker_runner, NULL);
	}
	for(int i=0; i<NUM_WORKERS, i++){
		pthread_join(workers[i], NULL);
	}


	exit(EXIT_SUCCESS) ;
}
