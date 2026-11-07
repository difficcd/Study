
typedef struct {
	int * route ;
	int * visited ;
	int next ;
} task_t ;

typedef struct {
	task_t ** elem ;
	int capacity ;
	int num ; 
	int front ;
	int rear ;

	int end ;   // task done 

	pthread_mutex_t lock;
	pthread_cond_t wait_enq;
	pthread_cond_t wait_deq;
} circular_queue ;

void circular_queue_init(circular_queue * buf, int capacity) ;

void circular_queue_enqueue(circular_queue * buf, task_t * task) ;
task_t * circular_queue_dequeue(circular_queue * buf) ;
