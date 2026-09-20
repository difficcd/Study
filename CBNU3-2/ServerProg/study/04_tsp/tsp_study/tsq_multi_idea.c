#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <pthread.h>
#include <signal.h>
#include <unistd.h>

#ifndef MAX
#define MAX 64
#endif

#define QUEUE_SIZE 4096


int weight[MAX][MAX];
int n_nodes = 0;
int min_weight_sum = 0;
struct timespec begin;

int best_route[MAX];
pthread_mutex_t min_lock = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t print_lock = PTHREAD_MUTEX_INITIALIZER; 

typedef struct {
    int route[MAX];
    int visited[MAX];
    int depth;
} Task;

Task task_queue[QUEUE_SIZE];
int queue_count = 0, in_ptr = 0, out_ptr = 0, is_finished = 0;
pthread_mutex_t q_lock = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t q_cond = PTHREAD_COND_INITIALIZER;

void push_task(Task t) {
    pthread_mutex_lock(&q_lock);
    while (queue_count == QUEUE_SIZE) {
        pthread_cond_wait(&q_cond, &q_lock);
    }
    task_queue[in_ptr] = t;
    in_ptr = (in_ptr + 1) % QUEUE_SIZE;
    queue_count++;
    pthread_cond_signal(&q_cond);
    pthread_mutex_unlock(&q_lock);
}

int pop_task(Task *t) {
    pthread_mutex_lock(&q_lock);
    while (queue_count == 0 && !is_finished) {
        pthread_cond_wait(&q_cond, &q_lock);
    }
    if (queue_count == 0 && is_finished) {
        pthread_mutex_unlock(&q_lock);
        return 0;
    }
    *t = task_queue[out_ptr];
    out_ptr = (out_ptr + 1) % QUEUE_SIZE;
    queue_count--;
    pthread_cond_signal(&q_cond);
    pthread_mutex_unlock(&q_lock);
    return 1;
}

int load_input(char *filepath) {
    char line[1024];
    FILE *fp;

    if (!(fp = fopen(filepath, "r"))) {
        printf("file open error");
        return 0;
    }

    if (fscanf(fp, "%[^\n]s", line) == 1) {
        for (char *t = strtok(line, " "); t != NULL; t = strtok(NULL, " ")) {
            weight[0][n_nodes] = atoi(t);
            n_nodes++;
        }
    }

    for (int i = 1; i < n_nodes; i++) {
        for (int j = 0; j < n_nodes; j++) {
            fscanf(fp, "%d", &weight[i][j]);
        }
    }

    fclose(fp);
    return n_nodes;
}

int _travel(int *route, int *visited, int next) {
    if (next == n_nodes) {
        int weight_sum = 0;
        for (int i = 0; i < n_nodes - 1; i++) {
            weight_sum += weight[route[i]][route[i + 1]];
        }
        weight_sum += weight[route[n_nodes - 1]][route[0]];

        pthread_mutex_lock(&min_lock);
        if (min_weight_sum == 0 || weight_sum < min_weight_sum) {
            struct timespec curr;
            clock_gettime(CLOCK_REALTIME, &curr);

            long int t = (curr.tv_sec - begin.tv_sec) * 1000000000 + (curr.tv_nsec - begin.tv_nsec);

            min_weight_sum = weight_sum;
            memcpy(best_route, route, sizeof(int) * n_nodes);

            pthread_mutex_lock(&print_lock);
            printf("%04ld.%09ld: %d [", t / 1000000000, t % 1000000000, weight_sum);
            for (int i = 0; i < n_nodes; i++) {
                printf("%d%c", route[i], i < n_nodes - 1 ? ',' : ']');
            }
            printf("\n");
            fflush(stdout);
            pthread_mutex_unlock(&print_lock);
        } // min weight 갱신 
        pthread_mutex_unlock(&min_lock);
        return min_weight_sum;
    }

    for (int node = 0; node < n_nodes; node++) {
        if (visited[node]) continue;

        visited[node] = 1;
        route[next] = node;
        _travel(route, visited, next + 1);
        visited[node] = 0;
    }
    return min_weight_sum;
}

void *worker_func(void *arg) {
    Task task;
    while (pop_task(&task)) {
        _travel(task.route, task.visited, task.depth);
    }
    return NULL;
}

void sigint_handler(int sig) {
    pthread_mutex_lock(&min_lock);

    printf("\n=== Interrupted ===\n");
    printf("Min Weight: %d\nRoute: [", min_weight_sum);
    for (int i = 0; i < n_nodes; i++) {
        printf("%d%c", best_route[i], i < n_nodes - 1 ? ',' : ']');
    }
    printf("]\n");

    pthread_mutex_unlock(&min_lock);
    exit(0);
}

void generate_tasks(int num_threads) {
    for (int node1 = 0; node1 < n_nodes; node1++) {
        for (int node2 = 0; node2 < n_nodes; node2++) {
            if (node1 == node2) continue;

            Task t;
            memset(t.visited, 0, sizeof(t.visited));
            t.route[0] = node1;
            t.route[1] = node2;
            t.visited[node1] = 1;
            t.visited[node2] = 1;
            t.depth = 2;
            push_task(t);
        }
    }

    pthread_mutex_lock(&q_lock);
    is_finished = 1;
    
    for (int i = 0; i < num_threads; i++) 
        pthread_cond_signal(&q_cond);
    
    pthread_mutex_unlock(&q_lock);
}


void run_workers(int num_threads) {
    pthread_t *threads = malloc(sizeof(pthread_t) * num_threads);

    for (int i = 0; i < num_threads; i++) 
        pthread_create(&threads[i], NULL, worker_func, NULL);
    for (int i = 0; i < num_threads; i++) 
        pthread_join(threads[i], NULL);

    free(threads);
}




int main(int argc, char **argv) {
    if (argc != 3) {
        return EXIT_FAILURE;
    }

    int num_threads = atoi(argv[2]);
    if (num_threads <= 0) return EXIT_FAILURE;
    signal(SIGINT, sigint_handler); // ctrl+C

    if (!load_input(argv[1]))
        return EXIT_FAILURE;
    clock_gettime(CLOCK_REALTIME, &begin);

    generate_tasks(num_threads);
    run_workers(num_threads);

    return EXIT_SUCCESS;
}