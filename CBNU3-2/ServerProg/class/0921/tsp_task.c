#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

#ifndef MAX
#define MAX 64
#endif

#ifndef BOUND
#define BOUND 6
#endif

typedef struct {
	int * route ;
	int * visited ;
	int next ;
} task_t ;


int weight[MAX][MAX] ;
int n_nodes = 0 ;
int min_weight_sum = 0 ; // 0 : undefined



struct timespec begin ; 

int sign = 0 ;
void show_running () ;
int load_input (char * filepath) ;

int _travel_task (task_t * task) {
	if (task->next == n_nodes) {
		show_running() ;

		int weight_sum = 0 ;
		for (int i = 0 ; i < n_nodes - 1 ; i++) {
			weight_sum += weight[task->route[i]][task->route[i + 1]] ;
		}
		weight_sum += weight[task->route[n_nodes - 1]][task->route[0]] ;

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

int travel_task (task_t * task)
{
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
		memcpy(task->visited, visited, next * sizeof(int)) ;
		task->next = next ;

		travel_task(task) ; /*FIXME: This version still processes tasks sequentially. This must be fixed.*/
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
	if (argc != 2)
		return EXIT_FAILURE ;

	if (!load_input(argv[1]))
		return EXIT_FAILURE ;

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


