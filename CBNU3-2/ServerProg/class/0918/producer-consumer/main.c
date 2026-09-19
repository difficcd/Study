#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "circular_queue.h" 

void * run_producer (void * arg) ;
void * run_consumer (void * arg) ;

circular_queue * buf = 0x0 ;

int main() 
{
	pthread_t prod[5] ;
	pthread_t cons[5] ;
	int i ;

	buf = malloc(sizeof(circular_queue)) ;
	circular_queue_init(buf, 3) ;
	// buffer size 3 

	for (i = 0 ; i < 10 ; i++) {
		circular_queue_enqueue(buf, "Hello") ;
	}
	for (i = 0 ; i < 10 ; i++) {
		char * s ;
		s = circular_queue_dequeue(buf) ;
		if (s != 0x0) 
			printf("%s\n", s) ;

		// 큐가 기다렸다가 받도록 ...해야함.
	}

	return EXIT_SUCCESS ;
}


// producer
void * run_producer (void * arg)
{
	char * prefix = (char *) arg ;

	for (int i = 0 ; i < 4 ; i++) {
		char message[128] ;
		sprintf(message, "%s (%d)", prefix, i) ;
		circular_queue_enqueue(message) ;
	}

	free(arg) ;
	return NULL ;
}


// consumer
void * run_consumer (void * arg)
{
	char * prefix = (char *) arg ;

	for (int i = 0 ; i < 4 ; i++) {
		char * message ;
		circular_queue_dequeue(message) ;
		printf("%s (%d) %s\n", prefix, i, message);

	}
	
	free(arg) ;
	return NULL ;
}
