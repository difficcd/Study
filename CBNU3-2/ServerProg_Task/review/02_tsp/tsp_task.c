#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <pthread.h>
#include <signal.h>

#include "circular_queue.h"

#ifndef MAX
#define MAX 64
#endif

#ifndef BOUND
#define BOUND 6
#endif

#ifndef CAPACITY
#define CAPACITY 100
#endif


int weight[MAX][MAX] ;
int n_nodes = 0 ;
int min_weight_sum = 0; // 0 : undefined
struct timespec begin ; 

int sign = 0 ;
void show_running () ;
int load_input (char * filepath) ;


// multithreading args
circular_queue * buf;

int thread_cnt = 1;
pthread_t * workers;
pthread_mutex_t lock;


// lock contention => local min array[thread_cnt]
// gr17 3201(17sec;multi vs. 6sec;seq)
// gr17 3201(12~16sec;multi + local_min)

int * local_min ; 
int * best_route ;
int thread_idx;


int _travel_task (task_t * task, int thread_idx) {
	if (task->next == n_nodes) {
		// show_running() ;

		int weight_sum = 0 ;
		for (int i = 0 ; i < n_nodes - 1 ; i++) {
			weight_sum += weight[task->route[i]][task->route[i + 1]] ;
		}
		weight_sum += weight[task->route[n_nodes - 1]][task->route[0]] ;

		// update  local min weight
		if (local_min[thread_idx] == 0 || weight_sum < local_min[thread_idx]) {
			local_min[thread_idx] = weight_sum ;
		}
		else return 0; //shortcut

		pthread_mutex_lock(&lock);
		if (local_min[thread_idx] < min_weight_sum || min_weight_sum == 0){
			struct timespec curr ;
			clock_gettime(CLOCK_REALTIME, &curr) ;
			long int t = (curr.tv_sec - begin.tv_sec) * 1000000000 + (curr.tv_nsec - begin.tv_nsec) ;
			
			min_weight_sum = local_min[thread_idx];
			for(int i = 0; i < n_nodes; i++){
				best_route[i] = task->route[i];
			}

			printf("\b") ;
			printf("%04ld.%09ld: %d ", t / 1000000000, t % 1000000000, min_weight_sum) ;
			printf("[") ;
			for (int i = 0 ; i < n_nodes ; i++) {
				printf("%d%c", task->route[i], i < n_nodes -1 ? ',' : ']' ) ;
			}
			printf("\n") ;
			fflush(stdin) ;
			// sign = 0 ;
		}
		pthread_mutex_unlock(&lock);
		return 0 ; // exit (value 문제 아님)
	}

	task->next++ ;
	for (int node = 0 ; node < n_nodes ; node++) {
		if (task->visited[node]) 
			continue ;

		task->visited[node] = 1 ;
		task->route[task->next - 1] = node ;
		_travel_task(task, thread_idx) ;
		task->visited[node] = 0 ;
	}
	task->next-- ;
	return 0;
}

int travel_task (task_t * task, int thread_idx) {
	/*
	pthread_mutex_lock(&lock);
	printf("Task begins: ") ;
	printf("[") ;
	for (int i = 0 ; i < task->next ; i++) {
		printf("%d%c", task->route[i], i < task->next - 1 ? ',' : ']' ) ;
	}
	printf("\n") ;
	pthread_mutex_unlock(&lock);
	*/

	_travel_task(task, thread_idx) ;

	free(task->route) ;
	free(task->visited) ;
	free(task) ;

	// printf("Task ends.\n") ;
}

int _travel (int * route, int * visited, int next) {
	if (next == BOUND + 1) {
		task_t * task = malloc(sizeof(task_t)) ;
		task->route = calloc(n_nodes, sizeof(int)) ;
		memcpy(task->route, route, next * sizeof(int)) ;
		task->visited = calloc(n_nodes, sizeof(int)) ;
		memcpy(task->visited, visited, n_nodes * sizeof(int)) ;
		task->next = next ;

		// travel_task(task) ; 
		circular_queue_enqueue(buf, task);
		return 0;
	}

	next++ ;
	for (int node = 0 ; node < n_nodes ; node++) {
		if (visited[node]) 
			continue ;

		visited[node] = 1 ;
		route[next - 1] = node ;
		_travel(route, visited, next) ; // recursive 
		visited[node] = 0 ;
	}
	next-- ;
}

// main controller 
void travel () {
	clock_gettime(CLOCK_REALTIME, &begin) ;

	int route[MAX] ;
	int visited[MAX] = { 0 } ;

	for (int node = 0 ; node < n_nodes ; node++) {
		route[0] = node ;
		visited[node] = 1 ;
		_travel(route, visited, 1) ;
		visited[node] = 0 ;
	}

	pthread_mutex_lock(&buf->lock);
	buf->end = 1; 
	pthread_cond_broadcast(&buf->wait_deq);
	pthread_mutex_unlock(&buf->lock);
}

// thread function
void * worker (void * args) {
	while(1){
		task_t * task = circular_queue_dequeue(buf); // block

		int index = (int)(long)args;

		if(task == NULL) break;

		travel_task(task, index);
	}
	return NULL;
}

// signal handler
void signal_handler (int signum) {

	printf("\n ====== Ctrl C detected ====== \n");

	struct timespec curr ;
	clock_gettime(CLOCK_REALTIME, &curr) ;
	long int t = (curr.tv_sec - begin.tv_sec) * 1000000000 + (curr.tv_nsec - begin.tv_nsec) ;
	printf("\b") ;
	printf("%04ld.%09ld: %d ", t / 1000000000, t % 1000000000, min_weight_sum) ;
	printf("[") ;
	for (int i = 0 ; i < n_nodes ; i++) {
		printf("%d%c", best_route[i], i < n_nodes -1 ? ',' : ']' ) ;
	}
	printf("\n") ;

	exit(0);

}


int main (int argc, char ** argv) {
	if (argc != 3){
		printf("usage: ./tsp_task <tsp data> <thread_cnt>\n");
		return EXIT_FAILURE ;
	}

	if (!load_input(argv[1]))
		return EXIT_FAILURE ;
	
	signal(SIGINT, signal_handler);
	signal(SIGTERM, signal_handler);

	thread_cnt = atoi(argv[2]);
	thread_idx = 0;
	workers = malloc(sizeof(pthread_t) * thread_cnt);
	best_route = calloc(sizeof(int), n_nodes);
	local_min = calloc(sizeof(int), thread_cnt);

	buf = malloc(sizeof(circular_queue));
	pthread_mutex_init(&lock, NULL);
	circular_queue_init(buf, CAPACITY);

	for(int i = 0; i < thread_cnt; i ++) { 
		pthread_create(&workers[i], NULL, worker, (void *)(long)i);
	}

	travel() ;

	for(int i = 0; i < thread_cnt; i ++) { 
		pthread_join(workers[i], NULL);
	}

	// print final result 
	struct timespec curr ;
	clock_gettime(CLOCK_REALTIME, &curr) ;
	long int t = (curr.tv_sec - begin.tv_sec) * 1000000000 + (curr.tv_nsec - begin.tv_nsec) ;
	printf("\b") ;
	printf("%04ld.%09ld: %d ", t / 1000000000, t % 1000000000, min_weight_sum) ;
	
	printf("\n\n ==== all thread's task done ... ==== \n final route is: [");
	for (int i = 0 ; i < n_nodes ; i++) {
		printf("%d%c", best_route[i], i < n_nodes -1 ? ',' : ']' ) ;
	}
	printf("\n") ;

	// free heap resource 
	free(workers);
	free(best_route);
	free(local_min);

	free(buf->elem);
	free(buf);

	return EXIT_SUCCESS ;
}




void show_running () {
	if (sign == 0) {
		printf("|") ; 
	}
	else if (sign == 4096) {
		printf("\b\b/") ; 
	}
	else if (sign == 4096 * 2) {
		printf("\b\b-") ; 
	}
	else if (sign == 4096 * 3) {
		printf("\b\b\\") ;
	}
	else if (sign == 4096 * 4) {
		printf("\b\b|") ;
		sign = 0 ; 
	}
	else {
	}
	sign++ ;
}

int load_input (char * filepath) 
{
	char line[1024] ;
	int w ;
	char b ;
	FILE * fp ; 

	if (!(fp = fopen(filepath, "r"))) 
		return 0 ;

	fscanf(fp, "%[^\n]s", line) ;
	for (char * t = strtok(line, " ") ; t != NULL ; t = strtok(NULL, " ")) {
		weight[0][n_nodes] = atoi(t) ;
		n_nodes++ ;
	}

	for (int i = 1 ; i < n_nodes ; i++) {
		for (int j = 0 ; j < n_nodes ; j++) {
			fscanf(fp, "%d", &weight[i][j]) ;
		}
	}

	fclose(fp) ;

	return n_nodes ;
}


