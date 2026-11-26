#include <stdint.h>

#define MAX_NUMBER 10000000

/* all fields are in network byte order */
typedef 
	struct {
		uint32_t lo ;
		uint32_t hi ;
	}
	request_msg ;

typedef 
	struct {
		uint32_t count ;
	}
	response_msg ;
