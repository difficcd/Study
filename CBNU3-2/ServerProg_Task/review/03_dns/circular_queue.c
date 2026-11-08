#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>

#include "circular_queue.h" 

void 
circular_queue_init(circular_queue_t * queue, void * elements, int capacity, int elem_size) {

	queue->elem = elements ;
	queue->capacity = capacity ;
	queue->elem_size = elem_size ;
	queue->num = 0 ;
	queue->front = 0 ;
	queue->rear = 0 ;

	queue->end = 0 ;

	pthread_mutex_init(&queue->lock, NULL) ;
	pthread_cond_init(&queue->wait_for_non_full, NULL) ;
	pthread_cond_init(&queue->wait_for_non_empty, NULL) ;
}

void 
circular_queue_enqueue (circular_queue_t * queue, void * task) 
{
	pthread_mutex_lock(&queue->lock) ;
	while (!(queue->num < queue->capacity)) {
		pthread_cond_wait(&queue->wait_for_non_full, &queue->lock) ;
	}
	
	//queue->elem[queue->rear] = task ;  => 교수님 힌트
	memcpy(queue->elem + queue->rear * queue->elem_size, task, queue->elem_size);
	queue->rear = (queue->rear + 1) % queue->capacity ;
	queue->num += 1 ;

	pthread_cond_signal(&queue->wait_for_non_empty) ;
	pthread_mutex_unlock(&queue->lock) ;
}

void 
circular_queue_dequeue (circular_queue_t * queue, void * buf) 
{
	pthread_mutex_lock(&queue->lock) ;
	while (queue->num == 0 && queue->end == 0) {
		pthread_cond_wait(&queue->wait_for_non_empty, &queue->lock) ;
	}

	if(queue->num == 0 && queue->end == 1){
		pthread_mutex_unlock(&queue->lock);
		return;
	}
	
	//void * r = queue->elem[queue->front] ; => 교수님 힌트
	memcpy(buf, queue->elem + queue->front * queue->elem_size, queue->elem_size) ;
	queue->front = (queue->front + 1) % queue->capacity ;
	queue->num -= 1 ;

	pthread_cond_signal(&queue->wait_for_non_full) ;
	pthread_mutex_unlock(&queue->lock) ;	
}