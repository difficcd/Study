#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <pthread.h>
#include <semaphore.h>

#ifndef MAX
#define MAX 64
#endif

#ifndef BOUND
#define BOUND 6
#endif

// signal handler : tsq_multi_idea 참조
// thread num 보여주도록 출력 갱신해 보기

typedef struct {
	int * route ;
	int * visited ;
	int next ;
    // pthread_t * workers;
} task_t ;

int weight[MAX][MAX] ;
int n_nodes = 0 ;
int min_weight_sum = 0 ; // 0 : undefined
// 전역 변수 :mutex 보호가 필요함

struct timespec begin ; 
int sign = 0 ;

int thread_cnt = 3; 

void show_running () ;
int load_input (char * filepath) ;

sem_t sem;
pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;


int _travel_task (task_t * task) {
	if (task->next == n_nodes) {
		show_running() ;

		int weight_sum = 0 ;
		for (int i = 0 ; i < n_nodes - 1 ; i++) {
			weight_sum += weight[task->route[i]][task->route[i + 1]] ;
		}
		weight_sum += weight[task->route[n_nodes - 1]][task->route[0]] ;

        // 전역변수 갱신 => critical sec, mutex 필요
        pthread_mutex_lock(&lock);
		if (min_weight_sum == 0 || weight_sum < min_weight_sum) {
			struct timespec curr ;
			clock_gettime(CLOCK_REALTIME, &curr) ;

			long int t = (curr.tv_sec - begin.tv_sec) * 1000000000 + (curr.tv_nsec - begin.tv_nsec) ;

			min_weight_sum = weight_sum ;
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
        pthread_mutex_unlock(&lock);
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

    return 0; // 표준
}

// task begin & end manage 
void * travel_task (void * arg)
{
    task_t * task = (task_t*) arg;
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

    sem_post(&sem);
	printf("Task ends.\n") ;

    return NULL;
}


int _travel (int * route, int * visited, int next) {
	if (next == BOUND + 1) {

		task_t * task = malloc(sizeof(task_t)) ;
        
		task->route = calloc(n_nodes, sizeof(int)) ;
		memcpy(task->route, route, next * sizeof(int)) ;

		task->visited = calloc(n_nodes, sizeof(int)) ;
		memcpy(task->visited, visited, next * sizeof(int)) ;
		task->next = next ;

        /*FIXME: This version still processes tasks sequentially. This must be fixed.*/
		// travel_task(task) ; 
        
        sem_wait(&sem);
    
        pthread_t worker;
        pthread_create(&worker, NULL, travel_task, task);
        pthread_detach(worker);
        // join 을 하면 bottleneck 생기니 detach로 OS가 알아서 처리하도록 관리
        // sem으로 thread_cnt 만큼 스레드 생성한 후 자원회수*post 위탁 관리

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
}

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
}

int main (int argc, char ** argv) {
    // ./name  <file>  <thread cnt>

	if (argc != 3) 
		return EXIT_FAILURE ;

	if (!load_input(argv[1]))
		return EXIT_FAILURE ;

    thread_cnt = atoi(argv[2]);
    sem_init(&sem, 0, thread_cnt);

	travel() ;

    pthread_exit(NULL); 
    // return 조기종료 방지
	// return EXIT_SUCCESS ;
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


