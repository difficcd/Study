#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "cache.h"
#include "circular_queue.h"
#include "rwlock.h"

#ifndef CAPACITY
#define CAPACITY 100
#endif


entry cache = { 0, 0, 0 } ; // global (sync)

typedef struct {
    char * url ;
    char * ip ;
    int update ; // 0:look, 1:update
} task_t;

rwlock_t rwlock;
circular_queue_t * queue ;

int thread_cnt ;


// thread start routine 
void * worker (void * args) {

    while(1) {
        task_t * task = NULL;
        circular_queue_dequeue(queue, &task);

        // 종료 조건 : deq return; task가 NULL인 경우
        // deq return 조건 : enqueue 끝났고, queue->num 0인 경우
        if(task == NULL) {
            break;
        }

        if(task->update == 1){

            write_lock(&rwlock);
            entry * e = update_entry(&cache, task->url, task->ip) ;

            if (e != NULL) {
                printf("%s %s\n", e->url, e->ip) ;
            }
            write_unlock(&rwlock);
        }   
        else if (task->update == 0){

            read_lock(&rwlock);
            entry * e = lookup_entry(&cache, task->url) ;

            if (e != NULL) {
                printf("%s %s\n", e->url, e->ip) ;
            }
            read_unlock(&rwlock);
        }
        
        free(task->url);
        free(task->ip);
        free(task);
    }

    return NULL;

}



int main (int argc, char ** argv)
{
	if (argc != 3) {
		fprintf(stderr, "Usage : %s <workload> <thread_cnt>\n", argv[0]) ;
		return EXIT_FAILURE ;
	}

    // 캐시 올리는거까지는 싱글, 그 이후를 멀티스레딩 
    // => 왜? 효율상... 어차피 멀티스레딩 해봤자 얘도 write 라서 경합생김.
    // add cache entry
	FILE * fp ; 
	if ((fp = fopen("cache", "r"))== NULL) {
		return EXIT_FAILURE ;
	}
	for (entry * last = &cache ; !feof(fp) ; ) {
		char url[256] ;
		char ip[256] ;

		fscanf(fp, "%256s %256s", url, ip) ;
		last = add_entry(last, url, ip) ;
	}
	fclose(fp) ;
	
	struct timespec begin ;
	clock_gettime(CLOCK_REALTIME, &begin) ;
    
    // queue init 
    queue = malloc(sizeof(circular_queue_t));
    void * elements = malloc(CAPACITY * sizeof(task_t *));
    circular_queue_init(queue, elements, CAPACITY, sizeof(task_t *)); 
    // deq에서는 void * buf 자리에 task * 를 pop 해줌
        // elem size 는 구조체의 sizeof()
        // elements 는 malloc 한 주소를 보내주어야 함. (init에서 할당 안함 + 시작주소)
        // elem[queue->rear] = task 방식이니까 task_t **가 맞음. 옵시디언 참조
    //

    // thread setting 
    rwlock_init(&rwlock);
    thread_cnt = atoi(argv[2]);
    pthread_t * workers = malloc(thread_cnt * sizeof(pthread_t));
    for(int i = 0; i < thread_cnt; i++) {
        pthread_create(&workers[i], NULL, worker, NULL);
    }

    // solve workload 
        // idea : lookup 이 대부분(read), update 는 적음(write)
        // 파일 loop에서 두 가지 갈래를 모두 멀티스레딩 rwlock 중심으로 ...
        // prod cons : task는 읽는 url, ip 기준.
        // update는 url, ip 모두 받고 lookup은 url만 보면 ip 나와야 하니까
        // flag 를 task에 같이 달아주는 것이 보기 좋음. entry 는 list자체니 재활용 불가.
	FILE * fw ;
	if (!(fw = fopen(argv[1], "r"))) 
		return EXIT_FAILURE ;

    // loop : workload task producer 
	while (feof(fw) == 0 ) {
		char opcode[16] ;
		char url[256] ;
		char ip[256] ;

		if (fscanf(fw, "%16s", opcode) != 1) 
			break ;

        // task_t * task = malloc(sizeof(task_t));
        // while문 내부에 있어야함. (task 개당 공간 할당 필요:enq memcpy단위)

		do {
			if (strcmp(opcode, "update") == 0) {
				if (fscanf(fw, "%256s %256s", url, ip) != 2) {
					fprintf(stderr, "Fail to read\n") ;
					break ; 
				}
                /*
                    entry * e = update_entry(&cache, url, ip) ;
                    if (e != NULL) {
                        printf("%s %s\n", e->url, e->ip) ;
                    }
                */
                task_t * task = malloc(sizeof(task_t));
                task->url = strdup(url);
                task->ip = strdup(ip);
                task->update = 1;

                circular_queue_enqueue(queue, &task);

			}

			if (strcmp(opcode, "lookup") == 0) {
				if (fscanf(fw, "%256s", url) != 1) {
					fprintf(stderr, "Fail to read\n") ;
					break ; 
				}
                /*
                    entry * e = lookup_entry(&cache, url) ;
                    if (e != NULL) {
                        printf("%s %s\n", e->url, e->ip) ;
                    }
                */
                task_t * task = malloc(sizeof(task_t));
                task->url = strdup(url);
                task->ip = NULL;
                task->update = 0;

                circular_queue_enqueue(queue, &task);
			}

			fprintf(fw, "Invalid operation: %s\n", opcode) ;
		} while (0) ;
	}
	fclose(fw) ;	

    // 모든 task의 enqueue 가 끝나면 set queue->end flag (1)
    pthread_mutex_lock(&queue->lock);
    queue->end = 1;
    pthread_cond_broadcast(&queue->wait_for_non_empty);
    pthread_mutex_unlock(&queue->lock);

    for(int i = 0; i < thread_cnt; i++) {
        pthread_join(workers[i], NULL);
    }

    free(queue);
    free(elements);
    free(workers);

	struct timespec curr ;
	clock_gettime(CLOCK_REALTIME, &curr) ;
	long int t = (curr.tv_sec - begin.tv_sec) * 1000000000 + (curr.tv_nsec - begin.tv_nsec) ;
	printf("%04ld.%09ld\n", t / 1000000000, t % 1000000000) ;

	return EXIT_SUCCESS ;
}
