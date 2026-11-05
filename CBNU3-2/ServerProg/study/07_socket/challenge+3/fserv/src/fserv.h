#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <unistd.h>
#include <endian.h>

enum REQUEST_TYPES {
    LIST = 100,
    GET  = 200,
    PUT  = 300,
	N_REQUEST_TYPES = 4 
} ; 

enum RESPONSE_STATUS {
    SUCCESS = 0,
	FAILURE = 100,
} ; 

typedef 
	struct {
		uint16_t type ;
		uint16_t filepath_len ;
		char * filepath ;		
	}
	request_t ;

typedef 
	struct {
		uint16_t response_status ;
		uint16_t filepath_len ;
		char * filepath ;
	}
	response_t ;

ssize_t recv_bytes (int conn, char *buf, size_t len) ;
ssize_t send_bytes (int conn, char *buf, size_t len) ;


void send_request_header (int conn_fd, uint16_t type,
						  uint16_t filepath_len, char * filepath, 
						  uint64_t data_len,     char * data) ; 

int send_response_header (int conn_fd, uint16_t status,
							uint64_t data_len, char * data) ;
    
