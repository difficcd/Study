#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>

typedef struct {
    char ** elem;
    int capacity;
    int num; 
    int front;
    int rear;

    pthread_mutex_t lock; 
    pthread_cond_t wait_for_non_full; 
    pthread_cond_t wait_for_non_empty;
} circular_queue;

#ifdef CIRQ 
circular_queue * buf = 0x0;
#endif 

void circular_queue_init(circular_queue * buf, int capacity) {
    buf->capacity = capacity;
    buf->elem = (char **) calloc(sizeof(char *), capacity);
    buf->num = 0;
    buf->front = 0;
    buf->rear = 0;
    
    pthread_mutex_init(&buf->lock, NULL);
    pthread_cond_init(&buf->wait_for_non_full, NULL);
    pthread_cond_init(&buf->wait_for_non_empty, NULL);
}

void circular_queue_enqueue(circular_queue * buf, char * msg) {
    pthread_mutex_lock(&buf->lock);
    while (!(buf->num < buf->capacity)) {
        pthread_cond_wait(&buf->wait_for_non_full, &buf->lock); 
    }

    buf->elem[buf->rear] = msg;
    buf->rear = (buf->rear + 1) % buf->capacity;
    buf->num += 1;

    pthread_cond_signal(&buf->wait_for_non_empty);
    pthread_mutex_unlock(&buf->lock);
}

char * circular_queue_dequeue(circular_queue * buf) {
    char * r = 0x0;

    pthread_mutex_lock(&buf->lock);
    while (!(buf->num > 0)) {
        pthread_cond_wait(&buf->wait_for_non_empty, &buf->lock); 
    }

    r = buf->elem[buf->front];
    buf->front = (buf->front + 1) % buf->capacity;
    buf->num -= 1;

    pthread_cond_signal(&buf->wait_for_non_full); 
    pthread_mutex_unlock(&buf->lock);

    return r;
}

#ifdef CIRQ
int main() {
    buf = malloc(sizeof(circular_queue));
    circular_queue_init(buf, 3);

    // 큐 용량이 3이므로, 
	// 넣고 꺼내는 것을 교대로 수행해야 블락(Deadlock)에 걸리지 않습니다.

    for (int i = 0; i < 10; i++) {
        circular_queue_enqueue(buf, "Hello");
        printf("Enqueued %d\n", i + 1);

        char * s = circular_queue_dequeue(buf);
        if (s != NULL) {
            printf("Dequeued: %s\n", s);
        }
    }

    exit(0);
}
#endif