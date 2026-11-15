#include <stdint.h>

typedef 
	enum {
		SUCCESS,
		FAIL
	}
	response_type ;

typedef 
	struct {
		uint8_t username[16] ;
		uint8_t password[16] ;
	}
	signin_msg ;

typedef 
	struct {
		uint64_t len ;
	}
	upstream_msg ;


typedef 
	struct {
		uint64_t timestamp ;
		uint8_t  username[16] ;
		uint64_t length ;
	}
	downstream_msg ;

typedef
	struct {
		response_type type ;
	}
	server_response ;