#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <pthread.h>
#include <signal.h>

#ifndef MAX
#define MAX 64
#endif

#ifndef BOUND
#define BOUND 6
#endif

// buffer capacity
#ifndef MAXCAPA
#define MAXCAPA 100
#endif



typedef struct {
	int * route ;
	int * visited ;
	int next ;
} task_t ;

// ==== circular_queue ==== //
typedef struct {
	task_t ** elem ;
	int capacity ;
	int num ; 
	int front ; 
	int rear ; 
	
	int end; // safe exit&join

	pthread_mutex_t lock;
	pthread_cond_t wait_deq;
	pthread_cond_t wait_enq;
} circular_queue ;
circular_queue * buf = 0x0 ;

int thread_cnt; // argv[2] 


int weight[MAX][MAX] ;
int n_nodes = 0 ;

// min_weight_sum : global min (threads=> renew global)
int min_weight_sum = 0; // 0 : undefined 
int best_route[MAX];  // global best_route

struct timespec begin ; 

int sign = 0 ;
void show_running () ;
int load_input (char * filepath) ;


// ==== lock contention manage  ==== //

int ** thread_local_min_route;  // [N_THREADs][MAX]
int * thread_local_min_weight;  // [N_THREADs]


// ==== signal_handler ==== //
 
void signal_handler(int signal) { // buf->end ?
	pthread_mutex_lock(&buf->lock);

	printf("\n\n ====== Cirl+C detected ======\nfinal route is: [");
	for (int i = 0 ; i < n_nodes ; i++) {
		printf("%d%c", best_route[i], i < n_nodes -1 ? ',' : ']' ) ;
	}
	printf("\nmin weight : %d \n", min_weight_sum);

	pthread_mutex_unlock(&buf->lock);

	exit(0);
}


// ==== prod cons ==== //
void 
circular_queue_init(circular_queue * buf, int capacity) {
	buf->capacity = capacity ;
	buf->elem = (task_t **) calloc(sizeof(task_t *), capacity) ;
	buf->num = 0 ;
	buf->front = 0 ;
	buf->rear = 0 ;

	buf->end = 0; // 1 == end flag, thread clean

	pthread_mutex_init(&buf->lock, NULL);
	pthread_cond_init(&buf->wait_deq, NULL);
	pthread_cond_init(&buf->wait_enq, NULL);
}

void 
circular_queue_enqueue(circular_queue * buf, task_t * task) {
	pthread_mutex_lock(&buf->lock);

	while(!(buf->num < buf->capacity)){
		pthread_cond_wait(&buf->wait_enq, &buf->lock);
	}
	buf->elem[buf->rear] = task ;
	buf->rear = (buf->rear + 1) % buf->capacity ; 
	buf->num += 1 ;

	pthread_cond_signal(&buf->wait_deq);
	pthread_mutex_unlock(&buf->lock);
}

task_t * 
circular_queue_dequeue(circular_queue * buf) {
	task_t * task = 0x0 ;

	pthread_mutex_lock(&buf->lock);

	while(!(buf->num > 0) && !buf->end){
		pthread_cond_wait(&buf->wait_deq, &buf->lock);
	}

	if(!(buf->num > 0) && buf->end){
		pthread_mutex_unlock(&buf->lock);
		return NULL;
	}

	task = buf->elem[buf->front] ;
	buf->front = (buf->front + 1) % buf->capacity ;
	buf->num -= 1 ;
	
	pthread_cond_signal(&buf->wait_enq);
	pthread_mutex_unlock(&buf->lock);

	return task ;
}



// ==== tsp logic ==== //

int _travel_task (task_t * task, int tid, int accumulate) {

	if (min_weight_sum != 0 && accumulate >= min_weight_sum) return 0; // 가지치기 추가 

	if (task->next == n_nodes) {

		int weight_sum = accumulate + weight[task->route[n_nodes-1]][task->route[0]];

		// ==== local min update ==== //
		if(thread_local_min_weight[tid] != 0 && weight_sum >= thread_local_min_weight[tid]) 
			return 0;  

		thread_local_min_weight[tid] = weight_sum; 
		memcpy(thread_local_min_route[tid], task->route, n_nodes * sizeof(int));
		

		// ==== global min update (min_weight_sum) ==== //

		// pthread_mutex_lock(&buf->lock);
		if (min_weight_sum == 0 || weight_sum < min_weight_sum) {
			struct timespec curr ;
			clock_gettime(CLOCK_REALTIME, &curr) ;
			long int t = (curr.tv_sec - begin.tv_sec) * 1000000000 + (curr.tv_nsec - begin.tv_nsec) ;

			// min_weight_sum = weight_sum ;
			
			pthread_mutex_lock(&buf->lock);
			if(!(min_weight_sum == 0 || weight_sum < min_weight_sum)) {
				pthread_mutex_unlock(&buf->lock); // 검사 안하면 deadlock
				return 0;
			} 
			// 재검사 필요 (time 측정 로직 도중에 Context Switching 위험)

			// global var update 
			min_weight_sum = weight_sum ;
			memcpy(best_route, task->route, n_nodes * sizeof(int));

			printf("\b") ;
			printf("%04ld.%09ld: %d ", t / 1000000000, t % 1000000000, weight_sum) ;
			printf("[") ;
			for (int i = 0 ; i < n_nodes ; i++) {
				printf("%d%c", task->route[i], i < n_nodes -1 ? ',' : ']' ) ;
			}
			printf("\n") ;
			fflush(stdout) ;
			
			// sign = 0 ;

			pthread_mutex_unlock(&buf->lock);
		}
		// pthread_mutex_unlock(&buf->lock);
		
		return min_weight_sum ;
	}

	task->next++ ;
	for (int node = 0 ; node < n_nodes ; node++) {
		if (task->visited[node]) 
			continue ;

		task->visited[node] = 1 ;
		task->route[task->next - 1] = node ;
		_travel_task(task, tid, accumulate + weight[task->route[task->next - 2]][node]) ;
		task->visited[node] = 0 ;
	}
	task->next-- ;
}

int travel_task (task_t * task, int tid) {
	/*
	printf("Task begins: ") ;
	printf("[") ;
	for (int i = 0 ; i < task->next ; i++) {
		printf("%d%c", task->route[i], i < task->next - 1 ? ',' : ']' ) ;
	}
	printf("\n") ;
	*/

    int init_weight = 0;
    for (int i = 0; i < task->next - 1; i++) {
        init_weight += weight[task->route[i]][task->route[i + 1]];
    }

	_travel_task(task, tid, init_weight) ;

	free(task->route) ;
	free(task->visited) ;
	free(task) ;

	// printf("Task ends.\n") ;
}

int _travel (int * route, int * visited, int next, int accumulate) {

	if(min_weight_sum != 0 && accumulate >= min_weight_sum) return 0; // 가지치기 추가 

	if (next == BOUND + 1) {                                                                                                                                                          
		task_t * task = malloc(sizeof(task_t)) ;

		task->route = calloc(n_nodes, sizeof(int)) ;
		memcpy(task->route, route, next * sizeof(int)) ;

		task->visited = calloc(n_nodes, sizeof(int)) ;
		memcpy(task->visited, visited, n_nodes * sizeof(int)) ;
		task->next = next ;

		// travel_task(task) ; 	
		
		circular_queue_enqueue(buf, task); // task enqueue
		return 0;
	}

	next++ ;
	for (int node = 0 ; node < n_nodes ; node++) {
		if (visited[node]) 
			continue ;

		visited[node] = 1 ;
		route[next - 1] = node ;
		
		int edge_weight;

		if (next > 2) 
			edge_weight= weight[route[next - 3]][node];
		else 
			edge_weight = 0;

        _travel(route, visited, next, accumulate + edge_weight) ;

		visited[node] = 0 ;
	}
	next-- ;
	return 0;
}


// ==== workers: thread functoin ==== //
void * work_travel(void * arg) {
	int tid = (int)(long)arg;

	while(1){
		task_t * task = circular_queue_dequeue(buf);	
		if(task) travel_task(task, tid) ;
		else return NULL;
	}
}


// dfs controller
void travel () {
	clock_gettime(CLOCK_REALTIME, &begin) ;

	// ==== buf, queue, workers(consumer), min_vars init ==== //
	buf = malloc(sizeof(circular_queue)) ;
	circular_queue_init(buf, MAXCAPA);
	
	pthread_t * workers = malloc(thread_cnt * sizeof(pthread_t));
	for(int i=0; i<thread_cnt; i++){
		pthread_create(&workers[i], NULL, work_travel, (void*)(long)i);
	} // tid == i

	thread_local_min_route = calloc(thread_cnt, sizeof(int*));
	for(int i=0; i<thread_cnt; i++){
		thread_local_min_route[i] = malloc(n_nodes * sizeof(int));
	}

	thread_local_min_weight = malloc(thread_cnt * sizeof(int));
    for(int i=0; i<thread_cnt; i++){
        thread_local_min_weight[i] = 0;
    }



	// ==== optimize : 가지치기 강화를 위해 0 대신 첫 번째 경로 weight로 init ==== // 
	int init_sum = 0;
    for (int i = 0; i < n_nodes - 1; i++) 
        init_sum += weight[i][i+1];
    init_sum += weight[n_nodes-1][0];
    
    min_weight_sum = init_sum; 
    for(int i=0; i<n_nodes; i++) best_route[i] = i;

	printf("start weight (initialized): %d [", min_weight_sum);
    for (int i = 0 ; i < n_nodes ; i++) {
        printf("%d%c", best_route[i], i < n_nodes - 1 ? ',' : ']');
    }
    printf("\n");
    fflush(stdout);
	

	// ==== dfs logic ==== //
	int route[MAX] ;
	int visited[MAX] = { 0 } ;

	/* for (int node = 0 ; node < n_nodes ; node++) {
		route[0] = node ;
		visited[node] = 1 ;
		_travel(route, visited, 1, 0) ;
		visited[node] = 0 ;
	} */

	route[0] = 0 ;
    visited[0] = 1 ;
    _travel(route, visited, 1, 0) ;
    visited[0] = 0 ;

	// ==== thread 자원 정리 ==== //
	pthread_mutex_lock(&buf->lock);
	buf->end = 1; 
	pthread_cond_broadcast(&buf->wait_enq);
	pthread_cond_broadcast(&buf->wait_deq);
	pthread_mutex_unlock(&buf->lock);

	for(int i=0; i<thread_cnt; i++){
		pthread_join(workers[i], NULL);
	}

	printf("\n\n ==== all thread's task doen ... ==== \n final route is: [");
	for (int i = 0 ; i < n_nodes ; i++) {
		printf("%d%c", best_route[i], i < n_nodes -1 ? ',' : ']' ) ;
	}
	printf("\n min weight : %d \n\n", min_weight_sum);



	// ==== local var 자원 정리 ==== //
	for(int i=0; i<thread_cnt; i++){
		free(thread_local_min_route[i]);
	}
	free(thread_local_min_route);
	free(thread_local_min_weight);

	free(workers);
	free(buf->elem);
	free(buf);
}



int main (int argc, char ** argv) {
	if (argc != 3)
		return EXIT_FAILURE ;

	if (!load_input(argv[1]))
		return EXIT_FAILURE ;

	thread_cnt = atoi(argv[2]); 
	// argv : char * 

	signal(SIGINT, signal_handler);
	travel() ;

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

int load_input (char * filepath) {
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


