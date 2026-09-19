#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
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

	for (i = 0 ; i < 5 ; i++) {
		/*FIXME Producer1, Producer2, Producer3, Producer4, Producer5 */
		char prefix[128] ;
		sprintf(prefix, "Producer%d", i + 1) ;
		pthread_create(&prod[i], NULL, run_producer, strdup(prefix)) ;

	}
	for (i = 0 ; i < 5 ; i++) {
		/*FIXME: Consumer1, Consumer2, Consumer3, Consumer4, Consumer5*/
		char prefix[128] ;
		sprintf(prefix, "Consumer%d", i + 1) ;
		pthread_create(&cons[i], NULL, run_consumer, strdup(prefix)) ;
	}
	/*FIXME: join*/
	for (int i = 0 ; i < 5 ; i++) {
		pthread_join(prod[i], NULL) ;
		pthread_join(cons[i], NULL) ;
	}

	free(buf) ;

	return EXIT_SUCCESS ;
}

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

void * run_consumer (void * arg)
{
	char * prefix = (char *) arg ;

	for (int i = 0 ; i < 4 ; i++) {
		char * message ;
		message = circular_queue_dequeue(buf) ;
		printf("%s (%d): %s\n", prefix, i, message) ;
	}
	
	free(arg) ;
	return NULL ;
}
