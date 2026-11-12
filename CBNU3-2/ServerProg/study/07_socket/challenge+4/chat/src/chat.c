#include <ncurses.h>
#include <pthread.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <stdatomic.h>

#include <arpa/inet.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include "chat.h"

pthread_t recipent_thread ;
pthread_t user_thread ;

WINDOW * outer_win = NULL ;
WINDOW * inner_win = NULL ;
WINDOW * input_win = NULL ;

int conn = -1 ;


// send helper
ssize_t send_bytes (int conn, char *buf, size_t len) 
{
    int current_len = 0, sent_bytes = 0;
	while(current_len < len){
		sent_bytes = send(conn, buf+current_len, len-current_len, 0);
		// printf("debug sent bytes : %d\n", sent_bytes);
		if(sent_bytes < 0){
			perror("string send: failed");
			return -1;
		}
		current_len += sent_bytes;
	}
	return current_len;
}

// recv helper 
ssize_t recv_bytes (int conn, char *buf, size_t len) 
{
    int current_len = 0, received_bytes = 0;
	while(current_len < len){
		received_bytes = recv(conn, buf+current_len, len-current_len, 0) ;
		// printf("debug recv bytes : %d\n", received_bytes);
		if(received_bytes == -1){
			perror("string receive: failed");
			return -1;
		}
		if(received_bytes == 0) {
			return 0;
		} 
		current_len += received_bytes;
	} // recv(conn, res, len, MSG_WAITALL); 
	return current_len;
}    



void window_teardown ()
{
	if (outer_win) delwin(outer_win) ;
	if (inner_win) delwin(inner_win) ;
	if (input_win) delwin(input_win) ;
	endwin() ;
}

void signal_handler (int signum) {
	if (signum == SIGINT || signum == SIGTERM) {
		window_teardown() ;
		exit(EXIT_SUCCESS) ;
	}
}

void * recipent_thread_runner (void * arg)
{
	if (conn == -1) return NULL ;

	int conn_fail_or_quit = 0 ;
	while (conn_fail_or_quit == 0) {
		/*TO-DO(9): receive a downstream message from the server */

		downstream_msg message ;
		char content[256] = "" ; // message : 256B ? (len: 0~2^64-1 B)

		if(!recv_bytes(conn, (char *) &message.timestamp, 8)){
			window_teardown() ;
			close(conn);
			printf("server closed\n");
			exit(0);
		}
        // recv_bytes(conn,        message.username, 16); 
        if(!recv_bytes(conn, (char *) &message.length, 8)){
			window_teardown() ;
			close(conn);
			printf("server closed\n");
			exit(0);
		}

		message.timestamp = be64toh(message.timestamp);
		message.length = be64toh(message.length);
		if(!recv_bytes(conn, content, message.length)){
			window_teardown() ;
			close(conn);
			printf("server closed\n");
			exit(0);
		} // recv payload (data)


		struct tm * timeinfo = localtime(&message.timestamp) ;
		
		wprintw(inner_win, "%02d:%02d:%02d ", \
							timeinfo->tm_hour, timeinfo->tm_min, timeinfo->tm_sec) ;
		// wprintw(inner_win, "[%s] ", message.username) ;
		// message.username : 설계상 들어갈 수 없어 str에 세팅된 상태로 수신

		wprintw(inner_win, "%s\n", content) ;
		wrefresh(inner_win) ;
	}
	return NULL ;
}

void * user_thread_runner (void * arg)
{
	int conn_fail_or_quit = 0 ;
	while (conn_fail_or_quit == 0) {
		char content[1024] = "" ;
		memset(content, 0, sizeof(content)); // 버퍼 오염 방지

		mvwgetnstr(input_win, 1, 2, content, 100) ; // 100~128 수준으로 
		wmove(input_win, 1,2) ;
		wclrtoeol(input_win) ;

		/*TO-DO(10): send a upstream message to the server */

		upstream_msg msg;
		msg.len = strlen(content);
		uint64_t send_msg_len = htobe64(msg.len);

		send_bytes(conn, (char *)& send_msg_len, 8);  // send message len
		send_bytes(conn, content, msg.len); 		  // send message 


		if (strcmp(content, "/quit") == 0) {
			conn_fail_or_quit = 1 ;
			// break ;
			exit(0);
		}
	}
}

void window_setup ()
{
	initscr() ;
	cbreak() ;
	curs_set(1) ;
	int rows, cols;
	getmaxyx(stdscr, rows, cols);
	int input_height = 3;
	int output_height = rows - input_height;
	outer_win = newwin(output_height, cols, 0, 0);
	inner_win = derwin(outer_win, output_height - 2, cols - 2, 1, 1) ;
	input_win = newwin(input_height, cols, output_height, 0); 
	scrollok(inner_win, TRUE) ;
	box(outer_win, 0, 0) ;
	box(input_win, 0, 0) ;
	mvwprintw(outer_win, 0, 2, " Chat") ;
	mvwprintw(input_win, 0, 2, " Text ") ;
	wrefresh(outer_win) ;
	wrefresh(input_win) ;
}

int main (int argc, char ** argv)
{
	atexit(window_teardown) ;
	signal(SIGINT, signal_handler) ;
	signal(SIGTERM, signal_handler)	;

	char username[16] = "" ;
	char password[16] = "" ;
	char hostname[32] = "" ;
	// max 15, 31 (예외처리 추가 필요함)

	int port = 0 ;

	if(argc != 9) {
        printf("Usage: ./chat -u <username> -s <password> -h <host> -p <portnum>\n");
        return 1;
    }
	
	int opt ;
	while ((opt = getopt(argc, argv, "u:s:h:p:")) != -1) {
		switch (opt) {
			case 'u':
				strncpy(username, optarg, 16) ;
				break ;
			case 's':
				strncpy(password, optarg, 16) ;
				break ;
			case 'h':
				strncpy(hostname, optarg, 32) ;
				break ;
			case 'p':
				port = atoi(optarg) ;
				break ;
			default:
				fprintf(stderr, "Invalid arguments\n") ;
				exit(EXIT_FAILURE) ;
		}		
	}
	if (username[0] == '\0' || password[0] == '\0' || hostname[0] == '\0' || port == 0) {
		fprintf(stderr, "Invalid arguments\n") ;
		exit(EXIT_FAILURE) ;
	}

	/* TO-DO(11): connection */
	conn = socket(AF_INET, SOCK_STREAM, 0);

    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_port = htons(port);

	if (inet_aton("127.0.0.1", &(addr.sin_addr)) == 0) {
		fprintf(stderr, "inet_aton(127.0.0.1, addr.sin_addr) returns zero\n") ;
		exit(EXIT_FAILURE) ;
	}
	if (connect(conn, (struct sockaddr *) &addr, sizeof(addr)) == -1) {
		perror("connect() fails");
		exit(EXIT_FAILURE);
	}


	// ==== send sign up message, recv server response ==== //
	signin_msg signin;
	strncpy(signin.username, username, 16);
	strncpy(signin.password, password, 16);

	signin.username[15] = '\0';
	signin.password[15] = '\0';

	send_bytes(conn, signin.username, 16);   // send username (16 * 1B)
	send_bytes(conn, signin.password, 16);   // send password (16 * 1B)


	server_response response ;
	uint32_t send_type;
	uint32_t recv_type;

	if(!recv_bytes(conn, (char *)& send_type, sizeof(server_response))){
		window_teardown() ;
		close(conn);
		printf("server closed\n");
		return 0;
	}

	if (sizeof(server_response) == 4)  
            recv_type = ntohl(send_type);
	if (sizeof(server_response) == 2)  
			recv_type = ntohs((uint16_t)send_type);

	if(recv_type == FAIL) {
		printf("login failed : wrong password or already signed in\n");
		return 1;
	}


	// ==== main logic ==== //
	
	window_setup() ;
	atomic_thread_fence(memory_order_seq_cst) ;
	// cache flush (memory 관리 시에 thread 분기나가면 문제발생 가능)
	// global var 들 setup 이후에 스레드들이 분기 ; 그 전에 fix됨(thread시에 read)
	// read는 해야하므로 cache 에 fix 해주어야 함

	pthread_create(&recipent_thread, NULL, recipent_thread_runner, NULL) ;
	pthread_create(&user_thread, NULL, user_thread_runner, NULL) ;
	pthread_join(recipent_thread, NULL) ;
	pthread_join(user_thread, NULL) ;

	window_teardown() ;
	close(conn);
	exit(EXIT_SUCCESS) ;
}