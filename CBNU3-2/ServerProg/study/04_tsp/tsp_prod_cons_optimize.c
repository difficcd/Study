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

#ifndef THREAD_BOUND
#define THREAD_BOUND 100
#endif


typedef struct {
    int * route ;
    int * visited ;
    int next ;
} task_t ;

int best_route[MAX];

typedef struct {
    task_t ** elem ;
    int capacity ;
    int num ;
    int front ;
    int rear ;

    int shutdown ;

    pthread_mutex_t lock ;
    pthread_cond_t wait_for_non_full ;
    pthread_cond_t wait_for_non_empty ;
} circular_queue ;

int weight[MAX][MAX] ;
int n_nodes = 0 ;
int min_weight_sum = 0 ;

struct timespec begin ; 
int sign = 0 ;

circular_queue * buf = NULL ; // cir queue 가져옴
int thread_cnt = 3 ;

pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER ;

void show_running () ;
int load_input (char * filepath) ;

int thread_local_min_route[N_THREADs][MAX];
// 얘도 sig 에서는 sync 해야함 == 시그널 핸들러에서도 락이 필요
// 왜? sync를 스레드 단위로 잘 해준다 해도 route memcpy중에 끊기면 깨진data나옴...

// thread 마다 lock 만들어서 sync 할땐 이거기준으로
// 후반부녹음 ck 


void circular_queue_init(circular_queue * buf, int capacity) {
    buf->capacity = capacity ;
    buf->elem = (task_t **) calloc(sizeof(task_t *), capacity) ;
    buf->num = 0 ;
    buf->front = 0 ;
    buf->rear = 0 ;

    buf->shutdown = 0 ; // thread 종료 여부 고지

    pthread_mutex_init(&buf->lock, NULL) ;
    pthread_cond_init(&buf->wait_for_non_full, NULL) ;
    pthread_cond_init(&buf->wait_for_non_empty, NULL) ;
}

void circular_queue_enqueue(circular_queue * buf, task_t * task) {
    pthread_mutex_lock(&buf->lock) ;

    while (!(buf->num < buf->capacity)) {
        pthread_cond_wait(&buf->wait_for_non_full, &buf->lock) ;
    }

    buf->elem[buf->rear] = task ;
    buf->rear = (buf->rear + 1) % buf->capacity ;
    buf->num += 1 ;

    pthread_cond_signal(&buf->wait_for_non_empty) ;
    pthread_mutex_unlock(&buf->lock) ;
}

task_t * circular_queue_dequeue(circular_queue * buf) {
    pthread_mutex_lock(&buf->lock) ;

    while (buf->num == 0 && !buf->shutdown) {
        pthread_cond_wait(&buf->wait_for_non_empty, &buf->lock) ;
    }

    if (buf->num == 0 && buf->shutdown) {
        pthread_mutex_unlock(&buf->lock) ;
        return NULL ;
    }

    task_t * task = buf->elem[buf->front] ;
    buf->front = (buf->front + 1) % buf->capacity ;
    buf->num -= 1 ;

    pthread_cond_signal(&buf->wait_for_non_full) ;
    pthread_mutex_unlock(&buf->lock) ;

    return task ;
}

int _travel_task (task_t * task) {
    if (task->next == n_nodes) {
        // show_running() ;

        int weight_sum = 0 ;
        for (int i = 0 ; i < n_nodes - 1 ; i++) {
            weight_sum += weight[task->route[i]][task->route[i + 1]] ;
        }
        weight_sum += weight[task->route[n_nodes - 1]][task->route[0]] ;

        if(thread_local_min != 0 && weight_sum >= thread_local_min);
            return 0;
            // 스레드 내부 기준의 local min 

            thread_local_min = weight_sum ; 

        // pthread_mutex_lock(&lock); 

        if (min_weight_sum == 0 || weight_sum < min_weight_sum) {
            struct timespec curr ;
            clock_gettime(CLOCK_REALTIME, &curr) ;

            long int t = (curr.tv_sec - begin.tv_sec) * 1000000000 + (curr.tv_nsec - begin.tv_nsec) ;

            // min_weight_sum = weight_sum ;
            
            // task 가 주어졌으면 그 본 값을 로컬하게 구한다음
            // 나중에 lock 을 한번만 잡아서 업뎃하거나 메인으로 보내서 메인에 업뎃 책임 넘김
            // 바운디드버퍼가 하나더 필요한 방식이기 때문에.. 단순하게 전역배열 두는게 좋음
            // 단 관리는 좀 어려움 ..

            memcpy(best_route, task->route, sizeof(int) * n_nodes);

            printf("\b") ;
            printf("%04ld.%09ld: %d ", t / 1000000000, t % 1000000000, weight_sum) ;
            printf("[") ;
            for (int i = 0 ; i < n_nodes ; i++) {
                printf("%d%c", task->route[i], i < n_nodes -1 ? ',' : ']' ) ;
            }
            printf("\n") ;
            fflush(stdout) ;
            sign = 0 ;
        }
        thread_local_min = min_weight;
        // 본거중에 제일 최소값 .... (lock 잡는 횟수가 매우 줄어들음.)

        // pthread_mutex_unlock(&lock);

        // lock 을 전체적으로 어떻게 잡을지?
        // 이렇게 해두면 route 개수만큼 lock 을 잡음.
        // n! 번 잡아서 seq 보다 좀 느리게 됨..
        // 여기서 이걸 어떻게 더 개선해야 하는가

        // memcpy minroute 를 route memcpy 하고...
        // 탐색하는 시간보다 더 많다고함. 모든 스레드가 minw 갱신에서다들 멈춤
        // 그래서 seq 하게 실행하는거랑 별차이없음

        // test and set을 할수 있다? 근데 그건 또 하면 안됨 (에러발생가능)
        // 큐에다가 min 값을 다 달아두고 마지막에 비교 갱신
        // 로컬 버전의 min w sum 만들기?

        // "스레드 기준, task 기준의 min w 을 각각 갱신하는 방법"
        // global var 말고 ... : 새 파일 참조


        return 0 ;
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

    return 0;
}

void travel_task_run (task_t * task) {
    pthread_t tid = pthread_self() ;
    printf("[Thread %lu] Task begins: [", (unsigned long)tid) ;
    for (int i = 0 ; i < task->next ; i++) {
        printf("%d%c", task->route[i], i < task->next - 1 ? ',' : ']' ) ;
    }
    printf("\n") ;

    _travel_task(task) ;
    
    free(task->route) ;
    free(task->visited) ;
    free(task) ;

    printf("[Thread %lu] Task ends.\n", (unsigned long)tid) ;
}


// 이거 주신 코드 보고 이름좀 통합하기 _task_manager
void * run_consumer (void * arg) {
    int thread_local_min = 0;
    // int thread_.... 위에 전역 이중배열...

    while (1) {
        task_t * task = circular_queue_dequeue(buf) ;
        if (task == NULL) 
            break ;
        travel_task_run(task) ;
    }
    return NULL ;
}

int _travel (int * route, int * visited, int next) {
    if (next == BOUND + 1) {
        task_t * task = malloc(sizeof(task_t)) ;
        
        task->route = calloc(n_nodes, sizeof(int)) ;
        memcpy(task->route, route, next * sizeof(int)) ;

        task->visited = calloc(n_nodes, sizeof(int)) ;
        memcpy(task->visited, visited, next * sizeof(int)) ;
        task->next = next ;

        circular_queue_enqueue(buf, task) ;
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

void sigint_handler(int sig) {

    printf("\n Ctrl C detected \n");
    printf("Min Weight: %d\nRoute: [", min_weight_sum);
    for (int i = 0; i < n_nodes; i++) {
        printf("%d%c", best_route[i], i < n_nodes - 1 ? ',' : ']');
    }
    printf("]\n");

    exit(0);
}


int main (int argc, char ** argv) {
    if (argc != 3)  return EXIT_FAILURE ;
    if (!load_input(argv[1])) return EXIT_FAILURE ;
    signal(SIGINT, sigint_handler); // SIGHAND

    thread_cnt = atoi(argv[2]);

    buf = malloc(sizeof(circular_queue)) ;
    circular_queue_init(buf, THREAD_BOUND) ;

    pthread_t * cons = malloc(sizeof(pthread_t) * thread_cnt) ;
    for (int i = 0 ; i < thread_cnt ; i++) {
        pthread_create(&cons[i], NULL, run_consumer, NULL) ;
    }

    travel() ;



    pthread_mutex_lock(&buf->lock) ;

    buf->shutdown = 1 ;
    pthread_cond_broadcast(&buf->wait_for_non_empty) ;
    // 종료 고지하고 cond var 전역 signal 주기

    pthread_mutex_unlock(&buf->lock) ;

    for (int i = 0 ; i < thread_cnt ; i++) {
        pthread_join(cons[i], NULL) ;
    }

    free(cons) ;
    free(buf->elem) ;
    free(buf) ;

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
    FILE * fp ; 

    if (!(fp = fopen(filepath, "r"))) 
        return 0 ;

    if (fgets(line, sizeof(line), fp) != NULL) {
        for (char * t = strtok(line, " \t\n\r") ; t != NULL ; t = strtok(NULL, " \t\n\r")) {
            weight[0][n_nodes] = atoi(t) ;
            n_nodes++ ;
        }
    }

    for (int i = 1 ; i < n_nodes ; i++) {
        for (int j = 0 ; j < n_nodes ; j++) {
            fscanf(fp, "%d", &weight[i][j]) ;
        }
    }

    fclose(fp) ;

    return n_nodes ;
}