#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
	char ** elem ;
	int capacity ;
	int num ; 
	int front ;
	int rear ;
} circular_queue ;

// circular_queue * buf = 0x0 ;

void 
circular_queue_init(circular_queue * buf, int capacity) {
	buf->capacity = capacity ;
	buf->elem = (char **) calloc(sizeof(char *), capacity) ;
	buf->num = 0 ;
	buf->front = 0 ;
	buf->rear = 0 ;
}

void // push
circular_queue_enqueue(circular_queue * buf, char * msg) 
{
	if (buf->num < buf->capacity) {
		buf->elem[buf->rear] = msg ;
		buf->rear = (buf->rear + 1) % buf->capacity ;
		buf->num += 1 ;
	}
}

char * // pop
circular_queue_dequeue(circular_queue * buf) 
{
	char * r = 0x0 ; // NULL
	if (buf->num > 0) {
		r = buf->elem[buf->front] ;
		buf->front = (buf->front + 1) % buf->capacity ;
		buf->num -= 1 ;
	}
	return r ;
	// 기다렸다가 바로 잡아야하는데 지금은 못잡음
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
*/