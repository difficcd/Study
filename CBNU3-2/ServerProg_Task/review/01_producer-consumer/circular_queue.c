#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>

typedef struct {
	char ** elem ;
	int capacity ;
	int num ; 
	int front ;
	int rear ;
} circular_queue ;

// circular_queue * buf = 0x0 ;
pthread_mutex_t lock;
pthread_cond_t wait_enq;
pthread_cond_t wait_deq;

void 
circular_queue_init(circular_queue * buf, int capacity) {
	buf->capacity = capacity ;
	buf->elem = (char **) calloc(sizeof(char *), capacity) ;
	buf->num = 0 ;
	buf->front = 0 ;
	buf->rear = 0 ;
	
	pthread_mutex_init(&lock, NULL);
	pthread_cond_init(&wait_enq, NULL);
	pthread_cond_init(&wait_deq, NULL);
}

void 
circular_queue_enqueue(circular_queue * buf, char * msg) 
{
	/*while(1){ // busy waiting 
		pthread_mutex_lock(&lock);
		if (buf->num < buf->capacity) {
			buf->elem[buf->rear] = msg ;
			buf->rear = (buf->rear + 1) % buf->capacity ;
			buf->num += 1 ;

			pthread_mutex_unlock(&lock);
			break;
		}
		pthread_mutex_unlock(&lock);
	}*/
	pthread_mutex_lock(&lock);
	while(!(buf->num < buf->capacity)){
		pthread_cond_wait(&wait_enq, &lock);
	}

	buf->elem[buf->rear] = msg ;
	buf->rear = (buf->rear + 1) % buf->capacity ;
	buf->num += 1 ;

	pthread_cond_signal(&wait_deq);
	pthread_mutex_unlock(&lock);
}

char * 
circular_queue_dequeue(circular_queue * buf) 
{
	char * r = malloc(sizeof(char) * 128) ;

	/* while(1){
		pthread_mutex_lock(&lock);
		if (buf->num > 0) {
			r = buf->elem[buf->front] ;
			buf->front = (buf->front + 1) % buf->capacity ;
			buf->num -= 1 ;

			pthread_mutex_unlock(&lock);
			break;
		}
		pthread_mutex_unlock(&lock);
	} */
	pthread_mutex_lock(&lock);
	while(!(buf->num > 0)){
		pthread_cond_wait(&wait_deq, &lock);
	}

	r = buf->elem[buf->front] ;
	buf->front = (buf->front + 1) % buf->capacity ;
	buf->num -= 1 ;

	pthread_cond_signal(&wait_enq);
	pthread_mutex_unlock(&lock);

	return r ;
}

/*
int 
main() 
{
	pthread_t prod[5] ;
	pthread_t cons[5] ;
	int i ;

	buf = malloc(sizeof(circular_queue)) ;
	circular_queue_init(buf, 3) ;

	for (i = 0 ; i < 10 ; i++) {
		circular_queue_queue(buf, "Hello") ;
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
*/