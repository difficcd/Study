#include <stdint.h>

#define TEAM_SIZE 3
#define MAX_NAME 31
#define MAX_MSG 128

/* every message is a header followed by "length" bytes of a string.
   "length" is in network byte order. */
typedef 
	struct {
		uint32_t length ;
	}
	message_header ;

	// 걍 똑같으니까 걍 안씀 