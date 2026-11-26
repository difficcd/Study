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
#define CHUNK_SIZE 4096

typedef 
	struct {
		/*TODO*/
		uint32_t offset;
		uint32_t length;
		char buf[CHUNK_SIZE];
	}
	task_t ;

/*TODO: define the data structure of a bounded buffer  => chunk.h */ 
/*TODO: define other global data structures */

circular_queue_t * buf = 0x0;

void queue_init(int capacity, int n){

}

task * get_task () 
{
	/*TODO*/
}

void put_task (uint32_t offset, uint32_t length)
{
	/*TODO*/
	task_t task = malloc(sizeof(task))
}

void * worker_runner (void * arg) 
{
	/*TODO*/
	// 큐에서 꺼내서 청크를 recv 하는 함수. (get_task사용 + 블로킹 + end flag)
	while(1){

	}

}

/* TODO: Add more functions if needed*/

/*
	A chunk server (given) sends a part of a file for each request. 
	Write a client that downloads a whole
	file in chunks of CHUNK_SIZE (4096) bytes using a thread pool.

	chunk 1/3 done
	chunk 2/3 done
	chunk 3/3 done
	done: 10000 bytes


	큰 파일을 나눠서 다운받는걸 display 하라는 것.
	서버의 dir 에 있는 file (-f) 내용 큰 걸 out(-o) 에 적어라 이거임
	근데 멀티스레딩이 섞인 => 어케함 ;;;;

	Do not keep the whole file in memory.
	Use a condition variable to print the lines in order. Busy waiting is not allowed.

	request 는 cli -> ser 쪽으로 보내야 함. 
	op가 1이면 (SIZE) file size 만 알려주고
	op 2면 (READ) n bytes 를 알려주고 n B 만큼의 data가 온다.

	op1 로 사이즈 알아내고
	그걸 쪼개서 task로 넣고
	op2 로  READ해서 멀티스레딩 처리 하라는 것.

*/ 

int send_bytes(int conn_fd, char * buf, int bytes){
	ssize_t current_len = 0, bytes_len = 0;
	while(current_len < bytes){
		bytes_len = send(conn_fd, buf+current_len, bytes-current_len, 0);
		if(bytes_len < 0){
			perror("send");
			exit(EXIT_FAILURE);
		}
		if(bytes_len == 0){
			return 1;
		}
		current_len += bytes_len;
	}
	return 0;
}

// return EOF
int recv_bytes(int conn_fd, char * buf, int bytes){
	ssize_t current_len = 0, bytes_len = 0;
	while(current_len < bytes){
		bytes_len = recv(conn_fd, buf+current_len, bytes-current_len, 0);
		if(bytes_len < 0){
			perror("recv");
			exit(EXIT_FAILURE);
		}
		if(bytes_len == 0){
			return 1; // EOF
		}
		current_len += bytes_len;
	}
	return 0;
}


void send_req(int conn_fd, request_msg request){

	uint32_t op 		= htonl(request.op);
	uint32_t namelen 	= htonl(request.namelen);
	uint32_t offset 	= htonl(request.offset);
	uint32_t length 	= htonl(request.length);

	send_bytes((char *) &op,      4);
	send_bytes((char *) &namelen, 4);
	send_bytes((char *) &offset,  4);
	send_bytes((char *) &length,  4);

	//printf("request ok\n");
}

void recv_res(int conn_fd, response_msg * response){

	uint32_t recv_type;	
	uint32_t recv_value; 
	recv_bytes((char *) &recv_type,  4);
	recv_bytes((char *) &recv_value, 4);

	response->type  = ntohl(recv_type );
	response->value = ntohl(recv_value);
	 
	//printf("response ok\n");
}


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
	conn_fd = socket(AF_INET, SOCK_STREAM, 0);
	
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

	request_msg size_request;
	size_request.op = 1;       size_request.namelen = strlen(file);
	size_request.offset = 0;   size_request.length = 0;
	send_req(size_request);
	send_bytes(file, strlen(file));
	printf("file : %s\n", file);

	response_msg size_response;
	recv_res(&size_response);
	printf("size response value: %d\n\n", size_response.value);

	// 한번에 실어오지 말고 offset부터 length 까지 받아오게 해라.
	// offset ~ length 까지 받아서 각각 저장하는 거까지만이라도 구현해 보기.
	// 아니네 사실 영역만 잘 나누면 하나의 buf를 나눠쓰는게 가능한데? 그래도 일단 분리해서..

	// task 에는 offset, length 랑 각자의 buf 가 있어야함.
	// offset이 사실상 chunk num 역할을 해준다.
	

	// 여기서 socket 을 여러개만들고 각각 connect를 해야함!!
	// 그래야 

	pthread_t workers[NUM_WORKERS];
	for(int i=0; i<NUM_WORKERS; i++)
		pthread_create(&workers[i], NULL, worker_runner, NULL);
	
	for(int i=0; i<NUM_WORKERS; i++)
		pthread_join(workers[i], NULL);


	exit(EXIT_SUCCESS) ;
}
