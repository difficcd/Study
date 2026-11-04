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

	for (i = 0 ; i < 10 ; i++) {
		/*FIXME Producer1, Producer2, Producer3, Producer4, Producer5 */
		circular_queue_queue(buf, "Hello") ;
	}
	for (i = 0 ; i < 10 ; i++) {
		/*FIXME: Consumer1, Consumer2, Consumer3, Consumer4, Consumer5*/
		char * s ;
		s = circular_queue_dequeue(buf) ;
		if (s != 0x0) 
			printf("%s\n", s) ;
	}

	return EXIT_SUCCESS ;
}

void * run_producer (void * arg)
{
	char * prefix = (char *) arg ;

	for (int i = 0 ; i < 4 ; i++) {
		char message[128] ;
		sprintf(message, "%s (%d)", prefix, i) ;
		circular_queue_queue(message) ;
	}

	free(arg) ;
	return NULL ;
}

void * run_consumer (void * arg)
{
	char * prefix = (char *) arg ;

	for (int i = 0 ; i < 4 ; i++) {
		char message[128] ;
		sprintf(message, "%s (%d)", prefix, i) ;
		circular_queue_queue(message) ;
	}
	
	free(arg) ;
	return NULL ;
}
