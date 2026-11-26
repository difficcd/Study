#include <stddef.h>
#include <stdint.h>

#define MAX_NAME 255
#define CHUNK_SIZE 4096


typedef 
	enum {
		OP_SIZE = 1,
		OP_READ = 2
	}
	op_type ;

typedef 
	enum {
		SUCCESS,
		FAIL
	}
	response_type ;

/* all fields are in network byte order */
typedef 
	struct {
		uint32_t op ;
		uint32_t namelen ;
		uint32_t offset ;
		uint32_t length ;
	}
	request_msg ;		/* followed by "namelen" bytes of the file name */

typedef 
	struct {
		uint32_t type ;
		uint32_t value ;
	}
	response_msg ;		/* OP_READ: followed by "value" bytes of data */


typedef struct {
	int elem; // 청크 번호가 elem 
	int capacity;
	int front;
	int rear; 
	
	pthread_mutex_t buf_lock;
	pthread_cond_t wait_deq;
	pthread_cond_t wait_enq;

} circular_queue_t;
/* transfer exactly "len" bytes. return 0 on success, -1 on error or disconnect (src/common.c) */
int send_bytes (int fd, const void * buf, size_t len) ;
int recv_bytes (int fd, void * buf, size_t len) ;
