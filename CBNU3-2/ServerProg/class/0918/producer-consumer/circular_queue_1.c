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

	pthread_mutex_t lock ; 
	pthread_cond_t wait_for_non_full ; 
	pthread_cond_t wait_for_non_empty ;
} circular_queue ;

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
	
	pthread_mutex_init(&buf->lock, NULL);
	pthread_cond_init(&buf->wait_for_non_full, NULL);
	pthread_cond_init(&buf->wait_for_non_empty, NULL);
}

void  // empty => nonempty (dequeue 조건 줄수 있음.)
circular_queue_enqueue(circular_queue * buf, char * msg) 
{
	//if (buf->num < buf->capacity) 
	// wait_for(buf->num < buf->capacity) ; 

	pthread_mutex_lock(&buf->lock) ;
	while(!(buf->num < buf->capacity)){ // while == until
		pthread_cond_wait(&buf->wait_for_non_full, &buf->lock) ; 
	}

	//assert( buf->num < buf->capacity);

	buf->elem[buf->rear] = msg ;
	buf->rear = (buf->rear + 1) % buf->capacity ;
	buf->num += 1 ;

	pthread_cond_signal(&buf->wait_for_non_empty) ;
	pthread_mutex_unlock(&buf->lock);

}

char *  // empty => nonempty (dequeue 조건 줄수 있음.)
circular_queue_dequeue(circular_queue * buf) 
{
	/*
	char * r = 0x0 ;
	if (buf->num > 0) {
		r = buf->elem[buf->front] ;
		buf->front = (buf->front + 1) % buf->capacity ;
		buf->num -= 1 ;
	}
	return r ;
	*/
	char * r = 0x0 ;

	// wait_for(buf->num > 0); 
	
	pthread_mutex_lock(&buf->lock) ;

	while(!(buf->num > 0)){ // while == until
		pthread_cond_wait(&buf->wait_for_non_empty, &buf->lock) ; 
	} // assert buf->num > 0

	r = buf->elem[buf->front] ;
	buf->front = (buf->front + 1) % buf->capacity ;
	buf->num -= 1 ;

	pthread_cond_signal(&buf->wait_for_non_full) ; 
	// broadcast 해도 되긴 함. 
	// 여기다가 클리어.. 추가해라 (큐를 날리거나 하는거) ==> 이런게 시험..
	// capa 두배로 중가 시키는 expand 같은거 구현하는거...
	// dequeue 를 한번에 두번, inqueue를 list 로 넣어라... 
	// 나혼자 잘 놀아봐야함.

	pthread_mutex_unlock(&buf->lock) ;

	return r;
}

#ifdef CIRQ
int 
main() 
{
	// pthread_t prod[5] ;
	// pthread_t cons[5] ;
	
	int i ;
	buf = malloc(sizeof(circular_queue)) ;
	circular_queue_init(buf, 3) ;

	for (i = 0 ; i < 10 ; i++) {
		circular_queue_enqueue(buf, "Hello") ;

		char * s ;
		s = circular_queue_dequeue(buf) ;
		if (s != 0x0) 
			printf("%s\n", s) ;
	}

	exit(0) ;
}
#endif
