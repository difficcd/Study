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

	buf = malloc(sizeof(circular_queue)) ; // 1개의 queue

	circular_queue_init(buf, 3) ; 
	// init capa == 3, 3개의 문자열까지만 들어가있을 수 있음

	for (i = 0 ; i < 10 ; i++) {
		circular_queue_enqueue(buf, "Hello") ;
	} // 10번 넣음 ==> enqueue 

	for (i = 0 ; i < 10 ; i++) {
		char * s ;
		s = circular_queue_dequeue(buf) ;
		if (s != 0x0) // NULL이 아닐 때만 print 
					  // (주석처리+%s=>%p 하면 s가 3회, NULL이 7회)
			printf("%s\n", s) ;
	}
	// thread 버전이 아닌 이 코드에서는
	// 단순히 capa 가 3이기 떄문에 3번만 들어가고 나머지는 블락됨

	return EXIT_SUCCESS ;
}


// 아래는 스레드를 활용하기 위한 prod, cons 함수들

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
		char message[128] ;
		sprintf(message, "%s (%d)", prefix, i) ;
		circular_queue_dequeue(buf) ;
	}
	
	free(arg) ;
	return NULL ;
}
