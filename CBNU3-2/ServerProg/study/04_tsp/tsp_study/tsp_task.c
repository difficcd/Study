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

// BOUND == 분할 seed 
// throughput 개선 : task를 더 적은 단위로 split
// 17도시 : 6도시 route에 넣어두고 11개 분기시켜서 멀티스레드

typedef struct {
	int * route ;
	int * visited ;
	int next ;

	// pthread_t thread;
} task_t ;
// thread 매개변수 args 를 위한 struct
// route[0]=1 : 첫 번째 방문 노드가 1번 도시

int weight[MAX][MAX] ;
int n_nodes = 0 ;
int min_weight_sum = 0 ; // 0 : undefined

struct timespec begin ; 

int sign = 0 ;
void show_running () ;
int load_input (char * filepath) ;


int _travel_task (task_t * task) {
	if (task->next == n_nodes) { // 순회 완료 시 sum처리
		show_running() ;

		int weight_sum = 0 ;
		for (int i = 0 ; i < n_nodes - 1 ; i++) {
			weight_sum += weight[task->route[i]][task->route[i + 1]] ;
		}
		weight_sum += weight[task->route[n_nodes - 1]][task->route[0]] ;
		// 현재 route의 가중치sum 갱신

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
		} // min > weight_sum 이면 갱신/출력
		return min_weight_sum ; // return 
	}

	task->next++ ; //  ( .., ..,  next + 1)
	for (int node = 0 ; node < n_nodes ; node++) {
		if (task->visited[node]) 
			continue ;

		// 아직 방문하지 않은 노드들을 각각 dfs 재귀
		task->visited[node] = 1 ;
		task->route[task->next - 1] = node ;
		_travel_task(task) ;
		task->visited[node] = 0 ;
	} // dfs 재귀 탐색
	task->next-- ; 
}

// _travel_task(실제처리) 포장함수 : teminal report + _travel_task + free
int travel_task (task_t * task) 
{
	printf("Task begins: ") ;
	printf("[") ;
	for (int i = 0 ; i < task->next ; i++) {
		printf("%d%c", task->route[i], i < task->next - 1 ? ',' : ']' ) ;
	} // 현재 조사할 route 출력해줌 (0~BOUND 노드까지)
	printf("\n") ; 

	_travel_task(task) ;  // BOUND 이후의 노드 분기 
	// pthread_create(&task->thread[i], NULL, _travel_task, (void**) &task)

	free(task->route) ;
	free(task->visited) ;
	free(task) ;

	printf("Task ends.\n") ;
	return 0;
}

// 시작노드가 있는 상태로 재귀 (travel 세팅=> _travel)
int _travel (int * route, int * visited, int next) {
	if (next == BOUND + 1) { 
		task_t * task = malloc(sizeof(task_t)) ; 

		// route, visited node기준으로 init
		task->route = calloc(n_nodes, sizeof(int)) ;
		memcpy(task->route, route, next * sizeof(int)) ;

		task->visited = calloc(n_nodes, sizeof(int)) ;
		memcpy(task->visited, visited, next * sizeof(int)) ;

		task->next = next ;

		travel_task(task) ; // BOUND까지 조사가 끝난 구조체 넘김
		
		/*FIXME: This version still processes tasks sequentially. This must be fixed.*/
	} // next가 끝까지 도달한 경우에만

	next++ ; 
	for (int node = 0 ; node < n_nodes ; node++) {
		if (visited[node]) 
			continue ;

		visited[node] = 1 ;
		route[next - 1] = node ;
		_travel(route, visited, next) ;
		// _travel(route, visited, next+1);

		visited[node] = 0 ;
	} // next 남아있으면 dfs 재귀
	next-- ;  
}

// travel => _travel => if(next == BOUND + 1):travel_task
void travel () { 
	clock_gettime(CLOCK_REALTIME, &begin) ;

	int route[MAX] ;
	int visited[MAX] = { 0 } ;
	// MAX 기준으로 선언 및 초기화

	// 시작점 setting 
	for (int node = 0 ; node < n_nodes ; node++) {
		route[0] = node ;
		visited[node] = 1 ;
		_travel(route, visited, 1) ;
		visited[node] = 0 ;
	}
}


// ==== main ==== //
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
	} // n_nodes = col=row count : 17, 21, 24, 48 

	for (int i = 1 ; i < n_nodes ; i++) {
		for (int j = 0 ; j < n_nodes ; j++) {
			fscanf(fp, "%d", &weight[i][j]) ;
		}
	}

	fclose(fp) ;

	return n_nodes ;
}


