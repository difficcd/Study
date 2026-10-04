#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "circular_queue.h"
#include "cache.h"

#ifndef MAXCAPA
#define MAXCAPA 100
#endif

#ifndef TASK
typedef struct {
    char * url ;
    char * ip ;
    int update;  // 1: update
} task_t ;
#endif
// add task struct

entry cache = { 0, 0, 0 } ;

circular_queue_t * queue = 0x0;
int thread_cnt = 0;


// producer (update, lookup workload)
void manager(char * url, char * ip, int update) 
{ 
	// task_t task; => task_t * task 유지(메모리 이점)
    task_t * task = malloc(sizeof(task_t));
	task->url = NULL;
	task->ip = NULL;

    if(url) task->url = strdup(url);
    if(ip)  task->ip = strdup(ip);

    task->update = update;

    circular_queue_enqueue(queue, task);
}

// consumer
void * worker(){
	entry * e;

	while(1){
		task_t * temp;
		circular_queue_dequeue(queue, &temp); 

		if(!temp) return NULL;
		if(temp->update == 1) {
			e = update_entry(&cache, temp->url, temp->ip) ;
			if (e != NULL) {
				printf("%s %s\n", e->url, e->ip) ;
			}
		}
		if(temp->update == 0) {
			e = lookup_entry(&cache, temp->url) ;
			if (e != NULL) {
				printf("%s %s\n", e->url, e->ip) ;
			}
		}

		free(temp->url);
		free(temp->ip);
		free(temp);
	}
}


int main (int argc, char ** argv)
{
	if (argc != 3) {
		fprintf(stderr, "Usage : %s <workload> <thread count>\n", argv[0]) ;
		return EXIT_FAILURE ;
	} // add thread count

	FILE * fp ; 
	if ((fp = fopen("cache", "r"))== NULL) {
		return EXIT_FAILURE ;
	}
	for (entry * last = &cache ; !feof(fp) ; ) {
		char url[256] ;
		char ip[256] ;

		fscanf(fp, "%256s %256s", url, ip) ;
		last = add_entry(last, url, ip) ;
		// sequential 유지 (write: lock contention)
	}
	fclose(fp) ;
	
	struct timespec begin ;
	clock_gettime(CLOCK_REALTIME, &begin) ;

	FILE * fw ;
	if (!(fw = fopen(argv[1], "r"))) 
		return EXIT_FAILURE ;


	// ==== buf, queue init ==== //
	queue = malloc(sizeof(circular_queue_t));
	void * elements = malloc(MAXCAPA * sizeof(task_t *));
    circular_queue_init(queue, elements, MAXCAPA, sizeof(task_t *));

	
	// ==== worker(consumer) thread setting ==== //
    thread_cnt = atoi(argv[2]);
    pthread_t * workers;
    workers = malloc(thread_cnt * sizeof(pthread_t));
	
	for(int i=0; i<thread_cnt; i++)
		pthread_create(&workers[i], NULL, worker, NULL);


	// ==== main loop ==== //
	while (feof(fw) == 0) {
		char opcode[16] ;
		char url[256] ;
		char ip[256] ;
		if (fscanf(fw, "%16s", opcode) != 1) 
			break ;

		do {
			if (strcmp(opcode, "update") == 0) {
				if (fscanf(fw, "%256s %256s", url, ip) != 2) {
					fprintf(stderr, "Fail to read\n") ;
					break ; 
				}

                manager(url, ip, 1); // ip: new_ip

				// entry * e = update_entry(&cache, url, ip);
				
				/*if (e != NULL) {
					printf("%s %s\n", e->url, e->ip) ;
				}*/
				break ;
			}
			if (strcmp(opcode, "lookup") == 0) {
				if (fscanf(fw, "%256s", url) != 1) {
					fprintf(stderr, "Fail to read\n") ;
					break ; 
				}

                manager(url, NULL, 0); // ip : NULL

				// entry * e = lookup_entry(&cache, url) ;
                
				/*if (e != NULL) {
					printf("%s %s\n", e->url, e->ip) ;
				}*/
				break;
			}

			fprintf(stderr, "Invalid operation: %s\n", opcode) ;
		} while (0) ;
	}
	fclose(fw) ;	

	for (int i = 0; i < thread_cnt; i++) {
		circular_queue_enqueue(queue, NULL);
	} // poison pill pattern

	for(int i=0; i<thread_cnt; i++)
		pthread_join(workers[i], NULL);

	free(workers);
	free(queue);
	free(elements);

	struct timespec curr ;
	clock_gettime(CLOCK_REALTIME, &curr) ;
	long int t = (curr.tv_sec - begin.tv_sec) * 1000000000 + (curr.tv_nsec - begin.tv_nsec) ;
	printf("%04ld.%09ld\n", t / 1000000000, t % 1000000000) ;

	return EXIT_SUCCESS ;
}
