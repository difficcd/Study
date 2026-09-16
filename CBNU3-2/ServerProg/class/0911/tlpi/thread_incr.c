/* thread_incr.c

   This program employs two POSIX threads that increment the same global
   variable, without using any synchronization method. As a consequence,
   updates are sometimes lost.

   See also thread_incr_mutex.c.
*/
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

static volatile int glob = 0;   /* "volatile" prevents compiler optimizations
                                   of arithmetic operations on 'glob' */
static void *                   /* Loop 'arg' times incrementing 'glob' */
thread_func(void *arg)
{
    int loops = *((int *) arg);
    int loc, j;

    for (j = 0; j < loops; j++) {
        loc = glob;
        loc++;
        glob = loc;
    }

    return NULL;
}
int
main(int argc, char *argv[])
{
    pthread_t t1, t2;
    int loops, s;

    loops = (argc > 1) ? atoi(argv[1]) : 10000000;

    s = pthread_create(&t1, NULL, thread_func, &loops);
    if (s != 0)
	    return EXIT_FAILURE ;

    s = pthread_create(&t2, NULL, thread_func, &loops);
    if (s != 0)
	    return EXIT_FAILURE ;

    s = pthread_join(t1, NULL);
    if (s != 0)
	    return EXIT_FAILURE ;

    s = pthread_join(t2, NULL);
    if (s != 0)
	    return EXIT_FAILURE ;


    printf("glob = %d\n", glob);
    exit(EXIT_SUCCESS);
}
