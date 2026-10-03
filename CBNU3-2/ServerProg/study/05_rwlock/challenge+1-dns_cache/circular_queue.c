#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>

#include "circular_queue.h" 

#ifdef CIRQ 
circular_queue_t * queue = 0x0 ;
#endif 


typedef struct {
    char * url ;
    char * ip ;
} task_t


// (circular_queue_t * queue, void * elements, int capacity, int elem_size) ; 
void 
circular_queue_init(circular_queue_t * queue, int capacity, void * elements, int elem_size) {

	task_t * elem = (task_t *) elements; 
	// elem_size 

	queue->capacity = capacity ;
	queue->elem = (task_t **) calloc(sizeof(task_t *), capacity) ;
	queue->num = 0 ;
	queue->front = 0 ;
	queue->rear = 0 ;

	pthread_mutex_init(&queue->lock, NULL) ;
	pthread_cond_init(&queue->wait_for_non_full, NULL) ;
	pthread_cond_init(&queue->wait_for_non_empty, NULL) ;
}

void 
circular_queue_enqueue(circular_queue_t * queue, void * msg) 
{
	task_t * task = (task_t *) msg ; 
	
	pthread_mutex_lock(&queue->lock) ;

	while (!(queue->num < queue->capacity)) {
		pthread_cond_wait(&queue->wait_for_non_full, &queue->lock) ;
	}
	queue->elem[queue->rear] = task ;
	queue->rear = (queue->rear + 1) % queue->capacity ;
	queue->num += 1 ;

	pthread_cond_signal(&queue->wait_for_non_empty) ;
	pthread_mutex_unlock(&queue->lock) ;
}

void *
circular_queue_dequeue(circular_queue_t * queue) 
{
	task_t * task = 0x0 ;

	pthread_mutex_lock(&queue->lock) ;

	while (!(queue->num > 0)) {
		pthread_cond_wait(&queue->wait_for_non_empty, &queue->lock) ;
	}
	task = queue->elem[queue->front] ;
	queue->front = (queue->front + 1) % queue->capacity ;
	queue->num -= 1 ;

	pthread_cond_signal(&queue->wait_for_non_full) ;
	pthread_mutex_unlock(&queue->lock) ;
	
	return (void *)task ;
}
