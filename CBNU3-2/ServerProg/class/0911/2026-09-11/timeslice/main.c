#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

void * worker (void * arg)
{
        pthread_t tid = pthread_self() ;

        struct timespec tp ;
        struct timespec prev = {0, 0} ;

        while (1) {
                clock_gettime(CLOCK_REALTIME, &tp) ;

                if (prev.tv_sec == 0 && prev.tv_nsec == 0) {
                        prev = tp ;
                        continue ;
                }

                printf("(%ld) %ld\n", tid % 100,
                        (tp.tv_sec - prev.tv_sec) * 1000000000 + (tp.tv_nsec - prev.tv_nsec)) ;

                prev = tp ;
        }

}


int main ()
{
        pthread_t t1 ;
        pthread_t t2 ;
        pthread_t t3 ;

        pthread_create(&t1, NULL, worker, NULL) ;
        pthread_create(&t2, NULL, worker, NULL) ;
        pthread_create(&t3, NULL, worker, NULL) ;

        pthread_join(t1, NULL) ;
        pthread_join(t2, NULL) ;
        pthread_join(t3, NULL) ;
}
