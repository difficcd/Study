#include <pthread.h>

typedef 
	struct {
		void * elem ;
		int capacity ;
		int elem_size ;
		int num ; 
		int front ;
		int rear ;
		
		pthread_mutex_t lock ;
		pthread_cond_t wait_for_non_full ;
		pthread_cond_t wait_for_non_empty ;
	}
	circular_queue_t ;

void circular_queue_init(circular_queue_t * queue, void * elements, int capacity, int elem_size) ;
void circular_queue_enqueue(circular_queue_t * queue, void * msg) ;
void * circular_queue_dequeue(circular_queue_t * queue, void * buf) ;