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

#include "chat.h"

pthread_t recipent_thread ;
pthread_t user_thread ;

WINDOW * outer_win = NULL ;
WINDOW * inner_win = NULL ;
WINDOW * input_win = NULL ;

int conn = -1 ;

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
		downstream_msg message ;
		char content[256] = "" ;

		/*TO-DO(9): receive a downstream message from the server */

		struct tm * timeinfo = localtime(&message.timestamp) ;
		wprintw(inner_win, "%02d:%02d:%02d ", \
							timeinfo->tm_hour, timeinfo->tm_min, timeinfo->tm_sec) ;
		wprintw(inner_win, "[%s] ", message.username) ;
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
		mvwgetnstr(input_win, 1, 2, content, 100) ; // 100~128 수준으로 
		wmove(input_win, 1,2) ;
		wclrtoeol(input_win) ;

		/*TO-DO(10): send a upstream message to the server */

		if (strcmp(content, "/quit") == 0) {
			conn_fail_or_quit = 1 ;
			break ;
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
	exit(EXIT_SUCCESS) ;
}