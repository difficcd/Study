
#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>

typedef struct {
    char ** elem; // 바뀌는 부분
    int capacity;
    int front;
    int rear;
    int num;

    pthread_mutex_t lock;
    pthread_cond_t wait_enq;
    pthread_cond_t wait_deq; 

} circular_queue_t;

void circular_queue_init(circular_queue_t * buf, int capacity){
    buf->elem = calloc(sizeof(char *), capacity); // 껍질만 calloc 사용시 elem[i] 할당
    buf->capacity = capacity;
    buf->front = 0;
    buf->rear = 0;
    buf->num = 0;

    pthread_mutex_init(&buf->lock, NULL);
    pthread_cond_init(&buf->wait_enq, NULL);
    pthread_cond_init(&buf->wait_deq, NULL);
}

void circular_queue_enq(circular_queue_t * buf, char * task){
    pthread_mutex_lock(&buf->lock);

    while(buf->num >= buf->capacity){
        pthread_cond_wait(&buf->wait_enq, &buf->lock);
    }
    buf->elem[buf->rear] = task;
    buf->rear = (buf->rear + 1) % buf->capacity;
    buf->num ++;
    pthread_cond_signal(&buf->wait_deq);

    pthread_mutex_unlock(&buf->lock);
}


char * circular_queue_deq(circular_queue_t * buf){
    pthread_mutex_lock(&buf->lock);

    while(buf->num <= 0){
        pthread_cond_wait(&buf->wait_deq, &buf->lock);
    }
    char * task = buf->elem[buf->front];
    buf->front = (buf->front + 1) % buf->capacity;
    buf->num --;
    pthread_cond_signal(&buf->wait_enq);

    pthread_mutex_unlock(&buf->lock);
    return task;
}

void *producer(void *arg) {
    circular_queue_t *buf = (circular_queue_t *)arg;
    for (int i = 0; i < 5; i++) {
        circular_queue_enq(buf, "Hello");
    }
    return NULL;
}

void *consumer(void *arg) {
    circular_queue_t *buf = (circular_queue_t *)arg;
    for (int i = 0; i < 5; i++) {
        char *s = circular_queue_deq(buf);
        if (s != NULL) {
            printf("%s\n", s);
        }
    }
    return NULL;
}

int main(void) {
    pthread_t prod[5];
    pthread_t cons[5];
    int i;

    circular_queue_t *buf = malloc(sizeof(circular_queue_t));
    if (buf == NULL) {
        perror("malloc");
        return 1;
    }

    circular_queue_init(buf, 3);

    for (i = 0; i < 5; i++) {
        if (pthread_create(&prod[i], NULL, producer, buf) != 0) {
            perror("pthread_create producer");
            return 1;
        }
        if (pthread_create(&cons[i], NULL, consumer, buf) != 0) {
            perror("pthread_create consumer");
            return 1;
        }
    }

    for (i = 0; i < 5; i++) {
        pthread_join(prod[i], NULL);
        pthread_join(cons[i], NULL);
    }

    free(buf);

    return 0;
}