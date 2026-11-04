#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>

#include "circular_queue.h"
// circular_queue * buf = 0x0 ;

void 
circular_queue_init(circular_queue * buf, int capacity) {
	buf->capacity = capacity ;
	buf->elem = (task_t **) malloc(sizeof(task_t *) * capacity) ;
	buf->num = 0 ;
	buf->front = 0 ;
	buf->rear = 0 ;
	
	pthread_mutex_init(&buf->lock, NULL);
	pthread_cond_init(&buf->wait_enq, NULL);
	pthread_cond_init(&buf->wait_deq, NULL);
}

void 
circular_queue_enqueue(circular_queue * buf, task_t * task) 
{
	/*while(1){ // busy waiting 
		pthread_mutex_lock(&lock);
		if (buf->num < buf->capacity) {
			buf->elem[buf->rear] = msg ;
			buf->rear = (buf->rear + 1) % buf->capacity ;
			buf->num += 1 ;

			pthread_mutex_unlock(&lock);
			break;
		}
		pthread_mutex_unlock(&lock);
	}*/
	pthread_mutex_lock(&buf->lock);
	while(!(buf->num < buf->capacity)){
		pthread_cond_wait(&buf->wait_enq, &buf->lock);
	}

	buf->elem[buf->rear] = task ;
	buf->rear = (buf->rear + 1) % buf->capacity ;
	buf->num += 1 ;

	pthread_cond_signal(&buf->wait_deq);
	pthread_mutex_unlock(&buf->lock);
}

task_t * 
circular_queue_dequeue(circular_queue * buf) 
{
	task_t * r ;

	/* while(1){
		pthread_mutex_lock(&lock);
		if (buf->num > 0) {
			r = buf->elem[buf->front] ;
			buf->front = (buf->front + 1) % buf->capacity ;
			buf->num -= 1 ;

			pthread_mutex_unlock(&lock);
			break;
		}
		pthread_mutex_unlock(&lock);
	} */
	pthread_mutex_lock(&buf->lock);
	while(!(buf->num > 0)){
		pthread_cond_wait(&buf->wait_deq, &buf->lock);
	}

	r = buf->elem[buf->front] ;
	buf->front = (buf->front + 1) % buf->capacity ;
	buf->num -= 1 ;

	pthread_cond_signal(&buf->wait_enq);
	pthread_mutex_unlock(&buf->lock);

	return r ;
}

