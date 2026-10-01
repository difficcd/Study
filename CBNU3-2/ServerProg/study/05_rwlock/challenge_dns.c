#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <pthread.h>
#include <signal.h>
#include <unistd.h>

typedef struct {
    int rlock_acquire ;
    int wlock_acquire ;
    int rlock_hold ;
    int wlock_hold ;

    pthread_mutex_t lock;
	pthread_cond_t wait_r;
	pthread_cond_t wait_w;
} rwlock_t ;


// ==== simple DNS cache dummy data ==== //
typedef struct {
    char * domain;
    char * ip; 
} dns_t;

dns_t test[2] = {
    {
        .domain = "google.com",
        .ip = "2404:6800:4005:825::200e",
        // => "2404:6800:4005:81a::200e" => ..
    },
    {
        .domain = "aws.amazon.com",
        .ip = "2600:9000:2891:9000:1c:a813:8500:93a1",
        // => "2600:9000:2891:6a00:1c:a813:8500:93a1" => ..
    }
};

struct timespec begin ; 
rwlock_t rwlock;


// helper function (for debugging)
void report (rwlock_t * m){

    printf("rlock_acquire : %d, rlock_hold : %d \n", m->rlock_acquire, m->rlock_hold);
    printf("wlock_acquire : %d, wlock_hold : %d \n\n", m->wlock_acquire, m->wlock_hold);
} 

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
        if(m->wlock_acquire > 0)
            pthread_cond_signal(&m->wait_w); 
        else 
            pthread_cond_broadcast(&m->wait_r);
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
    if(m->wlock_acquire > 0)
        pthread_cond_signal(&m->wait_w); 
    else 
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


 // sec:0...  thread [i]  : lookup google.com : ip = ....
void * user_request(void * arg){
    // 가상의 사용자 풀 : 0.1초당 1번씩 lookup 하는 reader thread들

    int tid = (int)(long)arg;
    unsigned int seed = tid;

    while(1){

        read_lock(&rwlock);
        struct timespec curr ;
        clock_gettime(CLOCK_REALTIME, &curr) ;
        long int t = (curr.tv_sec - begin.tv_sec) * 1000000000 + (curr.tv_nsec - begin.tv_nsec) ;

        if(tid < 5){ // 0~4 : google.com
            printf("sec: %04ld.%09ld   thread[%d] : lookup  %s  : ip = %s\n", 
                    t / 1000000000, t % 1000000000, tid, test[0].domain, test[0].ip);
        }
        else {       // 5~9 : aws.amazon.ocm
            printf("sec: %04ld.%09ld   thread[%d] : lookup  %s  : ip = %s\n", 
                    t / 1000000000, t % 1000000000, tid, test[1].domain, test[1].ip);
        }
        read_unlock(&rwlock);

        usleep(10000);
    }
}

void * load_balancer(void * arg){
    // 0.5초당 1번씩 addresses update 하는 writer thread들

    int tid = (int)(long)arg;
    unsigned int seed = tid;

    while(1){
        write_lock(&rwlock);
        struct timespec curr ;
        clock_gettime(CLOCK_REALTIME, &curr) ;
        long int t = (curr.tv_sec - begin.tv_sec) * 1000000000 + (curr.tv_nsec - begin.tv_nsec) ;

        if(tid < 5){
            if(!strcmp(test[0].ip, "2404:6800:4005:825::200e")) 
                test[0].ip = "2404:6800:4005:81a::200e";
            else test[0].ip = "2404:6800:4005:825::200e";

            printf("sec: %04ld.%09ld   thread[%d] : update  %s  : ip = %s\n", 
                    t / 1000000000, t % 1000000000, tid, test[0].domain, test[0].ip);
        }
        else {
            if(!strcmp(test[1].ip, "2600:9000:2891:9000:1c:a813:8500:93a1")) 
                test[1].ip = "2600:9000:2891:6a00:1c:a813:8500:93a1";
            else test[1].ip = "2600:9000:2891:9000:1c:a813:8500:93a1";

            printf("sec: %04ld.%09ld   thread[%d] : update  %s  : ip = %s\n", 
                    t / 1000000000, t % 1000000000, tid, test[1].domain, test[1].ip);
        }

        write_unlock(&rwlock);

        int random_sleep = 50000 + (rand_r(&seed) % 900000);
        usleep(random_sleep);
    }
    
}

int main(){

    init_rwlock(&rwlock);

    pthread_t readers[10];
    pthread_t workers[10];

    clock_gettime(CLOCK_REALTIME, &begin) ;

    for(int i=0; i<10; i++){
        pthread_create(&readers[i], NULL, user_request, (void *)(long)i);
    }

    for(int i=0; i<10; i++){
        pthread_create(&workers[i], NULL, load_balancer, (void *)(long)i);
    }

    
    for(int i=0; i<10; i++){
        pthread_join(readers[i], NULL);
    }

    for(int i=0; i<10; i++){
        pthread_join(workers[i], NULL);
    }

    return 0;
}

    // 목표 출력
    // sec:0.1 thread 0 : lookup google.com : ip = ....
    // sec:0.1  thread 1 : lookup google.com : ip = ....
    // sec:0.6  thread 6 : update google.com : ip = new ip
    // sec:0.8  thread 1 : lookup google.com : ip = new ip
    // .... 


// ==== challenge 후속 과제 ==== //

// 현재 내부적으로 구현해 둔 rwlock 인 rwlock_t 의 함수들을 사용하여
// 외부 프로그램에서 rwlock 사용하여 rw기반 멀티스레딩을 적용하는 것이 목표
// 여러 threads가 read, write를 하는 예시를 만들어 제출