#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>

#include "circular_queue.h" 

#ifdef CIRQ 
circular_queue * buf = 0x0 ;
#endif 

void 
circular_queue_init(circular_queue * buf, int capacity) {
	buf->capacity = capacity ;
	buf->elem = (char **) calloc(sizeof(char *), capacity) ;
	buf->num = 0 ;
	buf->front = 0 ;
	buf->rear = 0 ;

	pthread_mutex_init(&buf->lock, NULL) ;
	pthread_cond_init(&buf->wait_for_non_full, NULL) ;
	pthread_cond_init(&buf->wait_for_non_empty, NULL) ;
}

void 
circular_queue_enqueue(circular_queue * buf, char * msg) 
{
	pthread_mutex_lock(&buf->lock) ;

	//wait_for(buf->num < buf->capacity) ;

	while (!(buf->num < buf->capacity)) {
		pthread_cond_wait(&buf->wait_for_non_full, &buf->lock) ;
	}
		//pthread_mutex_unlock(&buf->lock) ;
		//pthread_mutex_lock(&buf->lock) ;

	// assert (buf->num < buf->capacity) ;
	buf->elem[buf->rear] = msg ;
	buf->rear = (buf->rear + 1) % buf->capacity ;
	buf->num += 1 ;

	pthread_cond_signal(&buf->wait_for_non_empty) ;
	pthread_mutex_unlock(&buf->lock) ;
}

char * 
circular_queue_dequeue(circular_queue * buf) 
{
	pthread_mutex_lock(&buf->lock) ;

	while (!(buf->num > 0)) {
		pthread_cond_wait(&buf->wait_for_non_empty, &buf->lock) ;
	}
	// assert buf->num > 0 

	char * r = 0x0 ;
	r = buf->elem[buf->front] ;
	buf->front = (buf->front + 1) % buf->capacity ;
	buf->num -= 1 ;

	pthread_cond_signal(&buf->wait_for_non_full) ;
	pthread_mutex_unlock(&buf->lock) ;
	
	return r ;
}

#ifdef CIRQ
int 
main() 
{
	pthread_t prod[5] ;
	pthread_t cons[5] ;
	int i ;

	buf = malloc(sizeof(circular_queue)) ;
	circular_queue_init(buf, 3) ;

	for (i = 0 ; i < 10 ; i++) {
		circular_queue_enqueue(buf, "Hello") ;
	}
	for (i = 0 ; i < 10 ; i++) {
		char * s ;
		s = circular_queue_dequeue(buf) ;
		if (s != 0x0) 
			printf("%s\n", s) ;
	}

	exit(0) ;
}
#endif
