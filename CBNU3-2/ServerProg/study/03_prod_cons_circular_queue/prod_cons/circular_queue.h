typedef struct {
	char ** elem ;
	int capacity ;
	int num ; 
	int front ;
	int rear ;
	pthread_mutex_t lock ;
	pthread_cond_t wait_for_non_full ;
	pthread_cond_t wait_for_non_empty ;
} circular_queue ;

void circular_queue_init(circular_queue * buf, int capacity) ;
void circular_queue_enqueue(circular_queue * buf, char * msg) ;
char * circular_queue_dequeue(circular_queue * buf) ;
