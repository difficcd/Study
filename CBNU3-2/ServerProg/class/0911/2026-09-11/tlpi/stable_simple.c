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

    free(arg); 
    pthread_exit((void*)r); 

    // pthread_exit((void *) strlen(s)) ;
    // return (void *) strlen(s);
}

int
main (int argc, char *argv[])
{
	pthread_t t1;
	long int *res;
	int s;

	char *a = strdup("hellow world");  
	/*
	char *a = (char *)malloc(strlen("hello") + 1);
	strcpy(a, "hello");	
	*/

	s = pthread_create(&t1, NULL, thread_func, a);
	
	if (s != 0)
		exit(EXIT_FAILURE) ;
	printf("Message from main()\n");

	s = pthread_join(t1, (void **)&res); 
	
	if (s != 0)
		exit(EXIT_FAILURE) ;
	
	printf("Thread returned %ld\n", *res); 
	free(res);

	exit(EXIT_SUCCESS);
}
