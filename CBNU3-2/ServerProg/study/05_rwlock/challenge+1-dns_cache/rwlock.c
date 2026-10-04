#include <stdio.h>
#include <stdlib.h>
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
} rwlock_t ;


void read_lock(rwlock_t * m){
    pthread_mutex_lock(&m->lock);

    m->rlock_acquire++; 
    while( m->wlock_hold || m->wlock_acquire ){
        pthread_cond_wait(&m->wait_r, &m->lock);
    }
    m->rlock_hold++;
    m->rlock_acquire--;

    pthread_mutex_unlock(&m->lock);
}


void read_unlock(rwlock_t *m){
    pthread_mutex_lock(&m->lock);

    m->rlock_hold--;
    if(m->rlock_hold == 0){
        pthread_cond_signal(&m->wait_w); 
    }

    pthread_mutex_unlock(&m->lock);
}


void write_lock(rwlock_t * m){
    pthread_mutex_lock(&m->lock);

    m->wlock_acquire++; // acquire 대기는 다수 가능
    while(m->wlock_hold || m->rlock_hold){
        pthread_cond_wait(&m->wait_w, &m->lock);
    }
    m->wlock_acquire--;
    m->wlock_hold = 1;

    pthread_mutex_unlock(&m->lock);
}

void write_unlock(rwlock_t * m){
    pthread_mutex_lock(&m->lock);

    m->wlock_hold = 0; 
    pthread_cond_broadcast(&m->wait_w); 
    pthread_cond_broadcast(&m->wait_r); 

    pthread_mutex_unlock(&m->lock);
}


void rwlock_init(rwlock_t * m){
    m->rlock_acquire = 0;
    m->wlock_acquire = 0;
    m->rlock_hold = 0;      // cocurrent
    m->wlock_hold = 0;      // mutual exclusion

    pthread_mutex_init(&m->lock, NULL);
    pthread_cond_init(&m->wait_r, NULL);
    pthread_cond_init(&m->wait_w, NULL);
}


