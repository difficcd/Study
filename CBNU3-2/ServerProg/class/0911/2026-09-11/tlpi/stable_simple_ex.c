/* simple_thread.c

   A simple POSIX threads example: create a thread, and then join with it.
*/

#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void *
thread_func(void *arg)
{
    char *s = arg;

    printf("s: %s, tid: %ld\n", s, (long)pthread_self());
    long int *r = malloc(sizeof(long int)); 
    *r = strlen(s);
	// 정수 수치 자체를 포인터로 강제 캐스팅하면 위험함(sys type 불일치)
	// 결과값을 저장할 정식 메모리 주소를 할당해서 return하도록 함.

    free(arg); // strdup agr를 안전하게 해제하기

    pthread_exit((void*)r); // 메모리 주소인 r 자체를 전달.

    // pthread_exit((void *) strlen(s)) ;
    // return (void *) strlen(s);
}

int
main (int argc, char *argv[])
{
	pthread_t t1;
	long int *res;
	int s;

	// readonly 에서 rw가능하게 (strdup = string duplicate)
	char *a = strdup("hellow world");  
	/*
	char *a = (char *)malloc(strlen("hello") + 1);
	strcpy(a, "hello");	
	*/
	
	// pthread_create
	s = pthread_create(&t1, NULL, thread_func, a);
	
	if (s != 0)
		exit(EXIT_FAILURE) ;

	printf("Message from main()\n");


	// pthread_join
	s = pthread_join(t1, (void **)&res); 
	// 스레드가 반환한 메모리주소 받기위해 void ** 이중포인터로 타입캐스팅
	
	if (s != 0)
		exit(EXIT_FAILURE) ;

	printf("Thread returned %ld\n", *res); // 진짜 결과값 출력
	free(res);

	exit(EXIT_SUCCESS);
}
