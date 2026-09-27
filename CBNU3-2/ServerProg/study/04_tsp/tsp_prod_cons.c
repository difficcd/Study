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
int min_weight_sum = 0 ; // 0 : undefined

struct timespec begin ; 

int sign = 0 ;
void show_running () ;
int load_input (char * filepath) ;


// ==== signal_handler ==== //

int final_route[MAX];

void signal_handler(int signal) {
	printf("\n\n ====== Cirl+C detected ======\nfinal route is: [");
	for (int i = 0 ; i < n_nodes ; i++) {
		printf("%d%c", final_route[i], i < n_nodes -1 ? ',' : ']' ) ;
	}

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



// ==== tsq logic ==== //

int _travel_task (task_t * task) {
	if (task->next == n_nodes) {
		// show_running() ;

		int weight_sum = 0 ;
		for (int i = 0 ; i < n_nodes - 1 ; i++) {
			weight_sum += weight[task->route[i]][task->route[i + 1]] ;
		}
		weight_sum += weight[task->route[n_nodes - 1]][task->route[0]] ;

		pthread_mutex_lock(&buf->lock);
		if (min_weight_sum == 0 || weight_sum < min_weight_sum) {
			struct timespec curr ;
			clock_gettime(CLOCK_REALTIME, &curr) ;

			long int t = (curr.tv_sec - begin.tv_sec) * 1000000000 + (curr.tv_nsec - begin.tv_nsec) ;

			min_weight_sum = weight_sum ;
			memcpy(final_route, task->route, sizeof(int) * n_nodes); // final_route record

			printf("\b") ;
			printf("%04ld.%09ld: %d ", t / 1000000000, t % 1000000000, weight_sum) ;
			printf("[") ;
			for (int i = 0 ; i < n_nodes ; i++) {
				printf("%d%c", task->route[i], i < n_nodes -1 ? ',' : ']' ) ;
			}
			printf("\n") ;
			fflush(stdin) ;
			sign = 0 ;
		}
		pthread_mutex_unlock(&buf->lock);
		
		return min_weight_sum ;
	}

	task->next++ ;
	for (int node = 0 ; node < n_nodes ; node++) {
		if (task->visited[node]) 
			continue ;

		task->visited[node] = 1 ;
		task->route[task->next - 1] = node ;
		_travel_task(task) ;
		task->visited[node] = 0 ;
	}
	task->next-- ;
}

int travel_task (task_t * task) {
	printf("Task begins: ") ;
	printf("[") ;
	for (int i = 0 ; i < task->next ; i++) {
		printf("%d%c", task->route[i], i < task->next - 1 ? ',' : ']' ) ;
	}
	printf("\n") ;

	_travel_task(task) ;

	free(task->route) ;
	free(task->visited) ;
	free(task) ;

	printf("Task ends.\n") ;
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
		
		circular_queue_enqueue(buf, task); // task enqueue
		return 0;
	}

	next++ ;
	for (int node = 0 ; node < n_nodes ; node++) {
		if (visited[node]) 
			continue ;

		visited[node] = 1 ;
		route[next - 1] = node ;
		_travel(route, visited, next) ;
		visited[node] = 0 ;
	}
	next-- ;
	return 0;
}


// ==== workers: thread functoin ==== //
void * work_travel() {
	while(1){
		task_t * task = circular_queue_dequeue(buf);	
		if(task) travel_task(task) ;
		else return NULL;
	}
}


// dfs controller
void travel () {
	clock_gettime(CLOCK_REALTIME, &begin) ;

	// ==== buf, queue, workers(consumer) init ==== //
	buf = malloc(sizeof(circular_queue)) ;
	circular_queue_init(buf, MAXCAPA);
	
	pthread_t * workers = malloc(thread_cnt * sizeof(pthread_t));
	for(int i=0; i<thread_cnt; i++){
		pthread_create(&workers[i], NULL, work_travel, buf);
	}

	// ==== dfs logic ==== //
	int route[MAX] ;
	int visited[MAX] = { 0 } ;

	for (int node = 0 ; node < n_nodes ; node++) {
		route[0] = node ;
		visited[node] = 1 ;
		_travel(route, visited, 1) ;
		visited[node] = 0 ;
	}

	// ==== thread 자원 정리 ==== //
	pthread_mutex_lock(&buf->lock);
	buf->end = 1; 
	pthread_cond_broadcast(&buf->wait_enq);
	pthread_cond_broadcast(&buf->wait_deq);
	pthread_mutex_unlock(&buf->lock);

	for(int i=0; i<thread_cnt; i++){
		pthread_join(workers[i], NULL);
	}

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


