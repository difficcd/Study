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

	// prod : prod1, prod2 ..
	for (i = 0 ; i < 5 ; i++) {
		// circular_queue_enqueue(buf, "Hello") ;

		char * prefix = malloc(128 * sizeof(char)); 
		snprintf(prefix, 128, "prod%d", i+1);
		pthread_create(&prod[i], NULL, run_producer, prefix);
	}

	// cons : cons1 cons2
	for (i = 0 ; i < 5 ; i++) {
		/*
		char * s ;
		s = circular_queue_dequeue(buf) ;
		if (s != 0x0) 
			printf("%s\n", s) ;
		*/

		char * prefix = malloc(128 * sizeof(char));
		snprintf(prefix, 128, "cons%d", i+1);
		pthread_create(&cons[i], NULL, run_consumer, prefix);
	}

	for(int i=0; i<5; i++){
		pthread_join(prod[i], NULL);
		pthread_join(prod[i], NULL);
	}

	free(buf->elem);
	free(buf);

	return EXIT_SUCCESS ;
}



void * run_producer (void * arg)
{
	char * prefix = (char *) arg ; 

	for (int i = 0 ; i < 4 ; i++) {
		char message[128] ;
		sprintf(message, "%s (%d)", prefix, i) ;
		circular_queue_enqueue(buf, strdup(message)) ;
	}

	free(arg) ;
	return NULL ;
}


void * run_consumer (void * arg)
{
	char * prefix = (char *) arg ;

	for (int i = 0 ; i < 4 ; i++) {
		char* message = circular_queue_dequeue(buf) ;
		printf("%s (%d): %s\n", prefix, i, message) ;
		free(message);
	}
	
	free(arg) ;
	return NULL ;
}
