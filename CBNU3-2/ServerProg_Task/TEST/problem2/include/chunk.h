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

