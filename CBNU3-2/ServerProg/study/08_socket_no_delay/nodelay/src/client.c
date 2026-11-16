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
#include <netinet/in.h>
#include <netinet/tcp.h>
#include <arpa/inet.h>

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
		char buf[512] ;
		ssize_t m ;
		memset(buf, 0, 512) ;
		m = recv(conn, buf, 512, 0) ;
		if (m > 0) {
			time_t tt = time(NULL) ;
			struct tm * t = localtime(&tt) ;
			wprintw(inner_win, "%02d:%02d:%02d ", t->tm_hour, t->tm_min, t->tm_sec) ; 
			wprintw(inner_win, "%s\n", buf) ;
			wrefresh(inner_win) ;
		}
	}
	return NULL ;
}

void * user_thread_runner (void * arg)
{
	wmove(input_win, 1, 2) ;
	int conn_fail_or_quit = 0 ;
	while (conn_fail_or_quit == 0) {
		char content[1024] = "" ;
		int c = wgetch(input_win) ;
		if (c == ERR) continue ;

		char s[16] ;
		s[0] = (char) c ;
		ssize_t m ;
		if ((m = send(conn, s, 1, 0)) > 0) {
			wmove(input_win, 1, 2) ;
			wprintw(input_win, ":%c", c) ;
			wrefresh(input_win) ;
			wclrtoeol(input_win) ;
		}
	}
}

void window_setup ()
{
	initscr() ;
	noecho() ;
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
	keypad(outer_win, TRUE) ;
	keypad(inner_win, TRUE) ;
	keypad(input_win, TRUE) ;

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

	char username[16] = "hongshin" ;
	char password[16] = "hongshin" ;
	char hostname[32] = "127.0.0.1" ;
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

	conn = socket(AF_INET, SOCK_STREAM, 0) ;
	if (conn < 0) {
		fprintf(stderr, "socket() fails.\n") ;
		exit(EXIT_FAILURE) ;
	}
	
	int flag = 1 ;
	if (setsockopt(conn, IPPROTO_TCP, TCP_NODELAY, &flag, sizeof(flag)) < 0) {
		fprintf(stderr, "setsockopt() fails.\n") ;
		exit(EXIT_FAILURE) ;
	}
	
	struct sockaddr_in srv_addr ;
	srv_addr.sin_family = AF_INET ;
	srv_addr.sin_port = htons(port) ;
	if (inet_aton("127.0.0.1", &(srv_addr.sin_addr)) == 0) {
		fprintf(stderr, "inet_aton(127.0.0.1, addr.sin_addr) returns zero\n") ;
		exit(EXIT_FAILURE) ;
	}
	if (connect(conn, (struct sockaddr *) &srv_addr, sizeof(srv_addr)) == -1) {
		fprintf(stderr, "connect() fails\n") ;
		exit(EXIT_FAILURE) ;
	}

	window_setup() ;

	atomic_thread_fence(memory_order_seq_cst) ;

	pthread_create(&recipent_thread, NULL, recipent_thread_runner, NULL) ;
	pthread_create(&user_thread, NULL, user_thread_runner, NULL) ;
	pthread_join(recipent_thread, NULL) ;
	pthread_join(user_thread, NULL) ;

	window_teardown() ;
	exit(EXIT_SUCCESS) ;
}
