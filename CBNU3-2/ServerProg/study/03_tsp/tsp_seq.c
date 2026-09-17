#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

#ifndef MAX
#define MAX 64
#endif

// MAX 매크로 상수가 이전에 정의되어 있지 않다면, 64로 정의
// 상수 중복 정의 에러 및 충돌 방지를 위한 것
// 전역 변수를 참조하는 매크로를 사용하기 때문에 필요한 처리


int weight[MAX][MAX] ; // 전역 정보저장 변수
int n_nodes = 0 ;

int min_weight_sum = 0 ; // 0 : undefined

struct timespec begin ; 

int sign = 0 ;


void show_running () 
{
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

	if (!(fp = fopen(filepath, "r"))) {
		printf("file open error");
		return 0 ;
	}

	fscanf(fp, "%[^\n]s", line) ;  // @1 
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



int _travel (int * route, int * visited, int next) 
{
	// 재귀 깊어질 때 : route 배열에는 방문"할"도시 번호 순서대로 적어둠

	if (next == n_nodes) { 
		// next == n_nodes : 재귀 마지막 노드 도달 시. (node < n_nodes ;)
		show_running() ;

		int weight_sum = 0 ;
		
		for (int i = 0 ; i < n_nodes - 1 ; i++) {
			weight_sum += weight[route[i]][route[i + 1]] ;
		}
		weight_sum += weight[route[n_nodes - 1]][route[0]] ;

		if (min_weight_sum == 0 || weight_sum < min_weight_sum) {
			struct timespec curr ;
			clock_gettime(CLOCK_REALTIME, &curr) ;

			// 시간 처리 (현재시간-시작시간)
			long int t = (curr.tv_sec - begin.tv_sec) * 1000000000 + (curr.tv_nsec - begin.tv_nsec) ;

			min_weight_sum = weight_sum ;
			printf("\b") ;
			printf("%04ld.%09ld: %d ", t / 1000000000, t % 1000000000, weight_sum) ; // 모소 시간, 
			printf("[") ;
			for (int i = 0 ; i < n_nodes ; i++) {
				printf("%d%c", route[i], i < n_nodes -1 ? ',' : ']' ) ;
			}
			printf("\n") ;
			fflush(stdin) ;
			sign = 0 ;
		}
		return min_weight_sum ;
	}

	for (int i = 0 ; i < next ; i++) {
		visited[route[i]] = 1 ;
	}
	for (int node = 0 ; node < n_nodes ; node++) {
		if (visited[node]) 
			continue ;

		// 아직 방문하지 않은 노드만 dfs 재귀
		visited[node] = 1 ;
		route[next] = node ;
		_travel(route, visited, next + 1) ;
		visited[node] = 0 ;
	}
}


void travel () 
{
	clock_gettime(CLOCK_REALTIME, &begin) ;  // @2

	int route[MAX] ;  // visited order record
	int visited[MAX] = { 0 } ;

	for (int node = 0 ; node < n_nodes ; node++) {
		route[0] = node ; 
		visited[node] = 1 ;
		_travel(route, visited, 1) ;

		visited[node] = 0 ; // initialization
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
