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

    printf("%s", s);
    pthread_exit((void *) strlen(s)) ; 
	// 정수인 strlen값을 void로 강제 형변환..
	// int void* 크기가 다를 때 data손실/타입불일치 생김. (역참조라도 했다간 crash)
	// 숫자 12라는 '값'을 "이거 메모리 주소 12번지야" 하고 속여서 보내는 것

    //return (void *) strlen(s);
}

int
main (int argc, char *argv[])
{
	pthread_t t1;
	void *res;
	int s;

	s = pthread_create(&t1, NULL, thread_func, "Hello world\n");
	if (s != 0)
		exit(EXIT_FAILURE) ;

	printf("Message from main()\n");
	s = pthread_join(t1, &res);
	if (s != 0)
		exit(EXIT_FAILURE) ;

	printf("Thread returned %ld\n", (long) res);

	exit(EXIT_SUCCESS);
}
