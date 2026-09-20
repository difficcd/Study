
typedef struct {
	char ** elem ;
	int capacity ;
	int num ; 
	int front ;
	int rear ;
} circular_queue ;

void circular_queue_init(circular_queue * buf, int capacity) ;

void circular_queue_enqueue(circular_queue * buf, char * msg) ;
char * circular_queue_dequeue(circular_queue * buf) ;
