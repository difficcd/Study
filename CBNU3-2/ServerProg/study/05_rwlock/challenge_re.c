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


int i;

// helper function (for debugging)
void report (rwlock_t * m){

    printf("[%d] rlock_acquire : %d, rlock_hold : %d \n", i, m->rlock_acquire, m->rlock_hold);
    printf("[%d] wlock_acquire : %d, wlock_hold : %d \n\n", i, m->wlock_acquire, m->wlock_hold);
    i++;
} 



void read_lock(rwlock_t * m){
    pthread_mutex_lock(&m->lock);

    m->rlock_acquire++; 
    report(m);
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
    report(m);

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


void init_rwlock(rwlock_t * m){
    m->rlock_acquire = 0;
    m->wlock_acquire = 0;
    m->rlock_hold = 0;      // cocurrent
    m->wlock_hold = 0;      // mutual exclusion

    pthread_mutex_init(&m->lock, NULL);
    pthread_cond_init(&m->wait_r, NULL);
    pthread_cond_init(&m->wait_w, NULL);
}


int main(){
    
    rwlock_t rwlock;

    init_rwlock(&rwlock);

    i=0;

    report(&rwlock); read_lock(&rwlock);
    report(&rwlock); read_lock(&rwlock);
    report(&rwlock); read_unlock(&rwlock);
    report(&rwlock); read_unlock(&rwlock);
    report(&rwlock); write_lock(&rwlock);
    // report(&rwlock); write_lock(&rwlock);
    report(&rwlock); write_unlock(&rwlock);
    report(&rwlock);

    return 0;
}

// ==== challenge 후속 과제 ==== //

// 현재 내부적으로 구현해 둔 rwlock 인 rwlock_t 의 함수들을 사용하여
// 외부 프로그램에서 rwlock 사용하여 rw기반 멀티스레딩을 적용하는 것이 목표
// 여러 threads가 read, write를 하는 예시를 만들어 제출