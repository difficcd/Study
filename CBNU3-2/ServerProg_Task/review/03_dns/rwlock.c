/*TODO*/
/* Requirements 
   - Concurrent Reads
   - Mutually Exclusive Write
   - Priorized Write
*/
#include <pthread.h>

typedef struct {
		int rlock_acquire ;
		int wlock_acquire ;
		int rlock_hold ;
		int wlock_hold ;

		pthread_mutex_t lock ;
		pthread_cond_t read_ready_cv ;
		pthread_cond_t write_ready_cv ;
	}
	rwlock_t ;

void rwlock_init(rwlock_t * m) {

    m->rlock_acquire = 0;
    m->wlock_acquire = 0;
    m->rlock_hold = 0;
    m->wlock_hold = 0;

    pthread_mutex_init(&m->lock, NULL);    
    pthread_cond_init(&m->read_ready_cv, NULL);
    pthread_cond_init(&m->write_ready_cv, NULL);

}

void read_lock(rwlock_t * m) {

    pthread_mutex_lock(&m->lock);

    m->rlock_acquire ++; 
    // mutex 범위 외에 있으면 acq처리 꼬일 수가 있음. 

    while (m->wlock_hold == 1 || m->wlock_acquire > 0) {
        pthread_cond_wait(&m->read_ready_cv, &m->lock);
    }

    // rlock holded (0~n) 
    m->rlock_acquire --;
    m->rlock_hold ++;

    pthread_mutex_unlock(&m->lock);

}

void read_unlock(rwlock_t * m) {

    // rlock_hold > 0 검사 필요한가? 
    // => mutex 로 묶어놔서 필요없고, 필요 없어야 함.

    pthread_mutex_lock(&m->lock);
    m->rlock_hold --;
    
    // rlock 을 unlock 하는 경우에는 대기중인 wlock만 깨워주면 됨.
    // broadcast, signal 둘 다 기능은 동일함 (w작업은 mutex개념)
    if(m->rlock_hold == 0 && m->wlock_acquire > 0){
        pthread_cond_signal(&m->write_ready_cv);
    }
    
    pthread_mutex_unlock(&m->lock);

}

void write_lock(rwlock_t * m) {

    pthread_mutex_lock(&m->lock);
    
    m->wlock_acquire ++; 

    while (m->wlock_hold == 1 || m->rlock_hold > 0) {
        pthread_cond_wait(&m->write_ready_cv, &m->lock);
    }

    // wlock holded (0,1) 
    m->wlock_acquire --;
    m->wlock_hold = 1;

    pthread_mutex_unlock(&m->lock);

}

void write_unlock(rwlock_t * m) {

    pthread_mutex_lock(&m->lock);
    m->wlock_hold --;
    
    // w unlock 시에는 먼저 w acq 에게 신호를 주고 (signal)
    // w 에 먼저 가게되면 w가 먼저 잡으니 broadcast 조건문 없어도됨.

    pthread_cond_signal(&m->write_ready_cv);
    pthread_cond_broadcast(&m->read_ready_cv);

    pthread_mutex_unlock(&m->lock);
}
