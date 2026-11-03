#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <sys/types.h>
#include <sys/socket.h>

#include <netinet/in.h>
#include <arpa/inet.h>

#include "fserv.h"

static char * REQUEST_COMMANDS[] = { "LIST", "GET", "PUT", NULL} ;


void send_request_header (int conn_fd, uint16_t type,
	               		  uint16_t filepath_len, char * filepath, 
				   		  uint16_t data_len,     char * data)
{
	// payload : req(100-dir, 200-dir, 300-dir,'data')
	// [type(2B)] [filepath_len(2B)] [filepath] (+ data:PUT)

	uint16_t send_type = htons(type);
	uint16_t send_filepath_len = htons(filepath_len);

	send_bytes(conn_fd, (char *)& send_type, 2);
	send_bytes(conn_fd, (char *)& send_filepath_len, 2);
	send_bytes(conn_fd, filepath, filepath_len);

	if(type == PUT && data_len != 0) {
		// PUT (payload : filepath + data)
		data_len = htons(data_len);
		send_bytes(conn_fd, (char *)& data_len, 2);
		send_bytes(conn_fd, data, data_len);
	}
}

int send_response_header (int conn_fd, uint16_t status,
						  uint16_t data_len, char * data)
{
	// uint16_t filepath_len, char * filepath, // 필요시 추가
	// [status(2B)] [data_len(2B)] [data]

	status = htons(status);
	uint16_t send_data_len = htons(data_len);

	send_bytes(conn_fd, (char *)& status, 2);
	send_bytes(conn_fd, (char *)& send_data_len, 2);
	send_bytes(conn_fd, data, data_len);

	if(status == FAILURE) {
		printf("Server response error\n");
		return 1;
	}
	return 0;
}


// recv, send bytes : success(return len), error(return -1)

ssize_t recv_bytes (int conn, char *buf, size_t len) 
{
    int current_len = 0, received_bytes = 0;
	while(current_len < len){
		received_bytes = recv(conn, buf+current_len, len-current_len, 0) ;
		printf("debug-recv-bytes : %d\n", received_bytes);
		if(received_bytes == -1){
			perror("string receive: failed");
			return -1;
		}
		if(received_bytes == 0) {
			break;
		} 
		current_len += received_bytes;
	} // recv(conn, res, len, MSG_WAITALL); 
	return current_len;
}    

ssize_t send_bytes (int conn, char *buf, size_t len) 
{
    int current_len = 0, sent_bytes = 0;
	while(current_len < len){
		sent_bytes = send(conn, buf+current_len, len-current_len, 0);
		printf("debug-sent-bytes : %d\n", sent_bytes);
		if(sent_bytes < 0){
			perror("string send: failed");
			return -1;
		}
		current_len += sent_bytes;
	}
	return current_len;
}