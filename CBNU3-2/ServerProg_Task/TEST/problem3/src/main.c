#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>
#include <pthread.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#include "prime.h"

#define MAXTHREAD 8
#define MAXCAPA  10

typedef 
	struct {
		/*TODO*/
		int conn_fd;
	}
	task_t ;

/*TODO: define the data structure of a bounded buffer */

typedef struct {
	task_t * elem;
	int num;
	int capacity;
	int front;
	int rear;

	pthread_mutex_t lock;
	pthread_cond_t wait_get;
	pthread_cond_t wait_put;
} circular_queue_t;

circular_queue_t * buf = 0x0;

/*TODO: define other global data structures */

void buf_init(circular_queue_t * buf, int capacity) 
{
	buf->elem = malloc(sizeof(task_t) * capacity);
	buf->num=0; buf->front=0; buf->rear=0;
	buf->capacity = capacity;

	pthread_mutex_init(&buf->lock, NULL);
	pthread_cond_init(&buf->wait_get, NULL);
	pthread_cond_init(&buf->wait_put, NULL);
}

task_t get_task (circular_queue_t * buf) 
{
	pthread_mutex_lock(&buf->lock);
	while(buf->num <= 0) {
		pthread_cond_wait(&buf->wait_get, &buf->lock);
	}
	task_t task;
	task = buf->elem[buf->front];
	buf->front = (buf->front + 1) % buf->capacity;
	buf->num --;
	pthread_cond_signal(&buf->wait_put);
	pthread_mutex_unlock(&buf->lock);

	return task;
}

void put_task (circular_queue_t * buf, task_t task)
{
	pthread_mutex_lock(&buf->lock);
	while(buf->num >= buf->capacity) {
		pthread_cond_wait(&buf->wait_put, &buf->lock);
	}
	buf->elem[buf->rear] = task;
	buf->rear = (buf->rear + 1) % buf->capacity;
	buf->num ++;
	pthread_cond_signal(&buf->wait_get);
	pthread_mutex_unlock(&buf->lock);
}


// send-recv helper
int send_bytes(int conn_fd, char * buf, int bytes){
	ssize_t current = 0, temp = 0;
	while(current < bytes){
		temp = send(conn_fd, buf+current, bytes-current, 0);
		if(temp < 0) {
			perror("send");
			return -1;
		}
		if(temp == 0) return 1; 
		current += temp;
	}
	return 0;
}

int recv_bytes(int conn_fd, char * buf, int bytes){
	ssize_t current = 0, temp = 0;
	while(current < bytes){
		temp = recv(conn_fd, buf+current, bytes-current, 0);
		if(temp < 0) {
			perror("recv");
			return -1;
		}
		if(temp == 0) return 1; // EOF
		current += temp;
	}
	return 0;
}

/* the number of prime numbers p such that lo <= p <= hi */
uint32_t count_primes (uint32_t lo, uint32_t hi)
{
	uint32_t count = 0 ;
	for (uint32_t n = (lo < 2 ? 2 : lo) ; n <= hi ; n++) {
		uint32_t d = 2 ;
		while (d * d <= n && n % d != 0)
			d++ ;
		if (d * d > n)
			count++ ;
	}
	return count ;
}

void * worker_runner (void * a) 
{
	/*TODO*/

	// put 으로 꺼내서 일 수행하기.
	
	while(1){ //  항상 일을 할 준비가 되어야 함.
		task_t task = get_task(buf);
		
		// 우선 recv 부터 (lo, hi 받음)
		uint32_t recv_lo, recv_hi ;
		if (recv_bytes(task.conn_fd, (char *) &recv_lo, 4) !=0){
			close(task.conn_fd);
			continue;
		}
		if (recv_bytes(task.conn_fd, (char *) &recv_hi, 4) !=0){
			close(task.conn_fd);
			continue;
		}

		uint32_t lo = ntohl(recv_lo);
		uint32_t hi = ntohl(recv_hi);
		printf("%d, %d\n", lo, hi);

		if(lo > hi || hi > MAX_NUMBER) {
			close(task.conn_fd);
			continue;
		}

		uint32_t count = count_primes(lo, hi);
		uint32_t send_count = htonl(count);
		send_bytes(task.conn_fd, (char *) &send_count, 4);

		close(task.conn_fd);
	}
}

// A client connects to the server
// through TCP and sends two integers lo and hi.

// The server replies with the number of prime numbers p
// such that lo ≤ p ≤ hi, and closes the connection.

int main (int argc, char ** argv)
{
	/*TODO*/

	// lo > hi or hi > MAX_NUMBER, the server closes the connection without a response.

	// ./bin/main -p <port> getopt 기억안나서 걍 argv[2]으로 씀

	if(argc != 3){
		printf("Usage: .//bin/main -p <port> \n");
		exit(EXIT_FAILURE);
	}

	int listen_fd = socket(AF_INET, SOCK_STREAM, 0);
	
	struct sockaddr_in addr;
	addr.sin_family = AF_INET;
	addr.sin_port = htons(atoi(argv[2]));

	if(inet_aton("127.0.0.1", &addr.sin_addr) == 0) {
		fprintf(stderr, "inet_aton() fails");
		exit(EXIT_FAILURE);
	}
	if(bind(listen_fd, (struct sockaddr *) &addr, sizeof(addr)) == -1){
		perror("bind");
		exit(EXIT_FAILURE);
	}
	if(listen(listen_fd, 2) == -1){
		perror("listen");
		exit(EXIT_FAILURE);
	}

	// The main thread puts accepted connections into the buffer, 
	// and the worker threads take them out.

	// connection 한다음 lo, hi 들어오면 그걸 버퍼에 넣고
	// worker 들은 그걸 꺼내씀

	// 여기서 init (buffer)
	buf = malloc(sizeof(circular_queue_t));
	buf_init(buf, MAXCAPA);

	pthread_t workers[MAXTHREAD];
	for(int i=0; i<MAXTHREAD; i++){
		pthread_create(&workers[i], NULL, worker_runner, NULL);
	}
	
	// put 기준은 ? => 연결 하나를 accept할 때마다 put 한 번입니다...

	do{
		int conn_fd = accept(listen_fd, NULL, NULL);
		task_t task;
		task.conn_fd = conn_fd;
		put_task(buf, task);

	} while(1);

	return EXIT_SUCCESS ;
}
