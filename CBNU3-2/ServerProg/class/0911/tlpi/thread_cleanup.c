/* thread_cancel.c
   Demonstrate the use of pthread_cancel() to cancel a POSIX thread.
   */

#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

static void
cleanup_handler (void * arg) 
{
	printf("clean-up\n") ;
}

void * thread_func (void *arg)
{
	int j;
	int ret;

	pthread_cleanup_push(cleanup_handler, NULL) ;

	printf("New thread started\n");     /* May be a cancellation point */
	for (j = 1; ; j++) {
		printf("Loop %d\n", j);         /* May be a cancellation point */
		sleep(1);                       /* A cancellation point */
	}

	pthread_cleanup_pop(&ret) ;

	return NULL;
}

int main (int argc, char *argv[])
{
	pthread_t thr;
	int s;
	void *res;

	s = pthread_create(&thr, NULL, thread_func, NULL);
	if (s != 0)
		return EXIT_FAILURE ;

	sleep(3);                           /* Allow new thread to run a while */

	s = pthread_cancel(thr);
	if (s != 0)
		return EXIT_FAILURE ;

	s = pthread_join(thr, &res);
	if (s != 0)
		return EXIT_FAILURE ;

	if (res == PTHREAD_CANCELED)
		printf("Thread was canceled\n");
	else
		printf("Thread was not canceled (should not happen!)\n");

	exit(EXIT_SUCCESS);
}
