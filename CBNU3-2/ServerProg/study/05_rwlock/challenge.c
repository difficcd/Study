#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <pthread.h>
#include <signal.h>

typedef struct {
    int rlock_acquire ;
    int wlock_acquire ;
    int rlock_hold ;
    int wlock_hold ;

    pthread_mutex_t lock;
	pthread_cond_t wait_r;
	pthread_cond_t wait_w;
}rwlock_t ;

// phtread_mutex_init(&m->lock, NULL);

void read_lock(rwlock_t * m){
    pthread_mutex_lock(&m->lock);

    m->rlock_acquire++ ; 
    
    if(!(m->wlock_hold && m->wlock_acqurie)){
        pthread_cond_wait(&m->wait_r);
    }

    m->rlock_hold++;
    m->rlock_acqurie--;

    pthread_mutex_unlock(&m->lock);
}

void read_unlock(rwlock_t *m){
    pthread_mutex_lock(&m->lock);

    if(rlock_hold == 0){
        pthread_cond_signal(&m->wait_w);
        pthread_cond_signal(&m->wait_r);
    }

    pthread_mutex_unlock(&m->lock)
}


void write_lock(rwlock_t * m){
    pthread_mutex_lock(&m->lock);

    m->wlock_acquire = 1;
    if(!(wlock_hold || rlock_hold)){
        
    }

    pthread_mutex_unlock(&m->lock)
}