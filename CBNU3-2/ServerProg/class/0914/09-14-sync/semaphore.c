#include <ncurses.h>
#include <pthread.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

pthread_t worker_1 ;
pthread_t worker_2 ;

WINDOW * outer_win ;
WINDOW * inner_win ;
WINDOW * input_win ;

pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t c = PTHREAD_COND_INITALIZER;

struct thread_context {
	int id ; 
	int cond ; 
	char * message ; 

	pthread_mutex_t lock ;
	pthread_cond_t cond ;
	
	sem_t sem ;
} ; 

sem = 1;

void * worker_thread_1 (void * arg)
{

	struct thread_context * ctx = (struct thread_context*) arg;
	while(1) {
		
		pthread_mutex_lock(&(ctx->lock)) ;
		while(1) {  

			sem_wait(&(ctx->sem)) ;
			sem_post(&(ctx->sem)) ;

			/*
			pthread_cond_wait(&(ctx->cond), &ctx->lock) ;
			// c에대해 lock 을 잡은상태에서블락이됨?
			// cond wait 할때 뮤텍스 입력받는이유:
			// wait으로 block 들어갈때 unlock해주고
			// 다시 락을 잡아야 하니깐 &lock 을 주는것임.
			// cond var 를 쓸때는 lock while cond_wait  unlock
			// 이렇게 써야만 함
		}
		pthread_mutex_unlock(&ctx->lock); */

		wprintw(inner_win, "(1) Hello\n") ;
		wrefresh(inner_win) ;
		sleep(1) ;
	}
}

void * worker_thread_2 (void * arg)
{
	while(1) {
		pthread_mutex_lock(&lock);
		wprintw(inner_win, "(2) Hi\n") ;
		wrefresh(inner_win) ;
		pthread_mutex_unlock(&lock);
		sleep(1) ;
	}
}



int main(void)
{
	initscr();
	cbreak();
	curs_set(1);

	int rows, cols;
	getmaxyx(stdscr, rows, cols);

	int input_height = 3;
	int output_height = rows - input_height;

	outer_win = newwin(output_height, cols, 0, 0);
	inner_win = derwin(outer_win, output_height - 2, cols - 2, 1, 1) ;

	WINDOW *input_win = newwin(input_height, cols, output_height, 0);

	scrollok(inner_win, TRUE);

	box(outer_win, 0, 0);
	box(input_win, 0, 0);

	mvwprintw(outer_win, 0, 2, " Output ");
	mvwprintw(input_win, 0, 2, " Command ");

	wrefresh(outer_win);
	wrefresh(input_win);

	pthread_create(&worker_1, NULL, worker_thread_1, NULL) ;
	pthread_create(&worker_2, NULL, worker_thread_2, NULL) ;

	int num ;
	while (1) {
		char s[128] ;
		mvwgetnstr(input_win, 1, 2, s, 127) ;
		wmove(input_win, 1,2) ;
		wclrtoeol(input_win) ;

		if (strcmp(s, "quit") == 0) {
			pthread_cancel(worker_1) ;
			pthread_cancel(worker_2) ;

			pthread_join(worker_1, NULL) ;
			pthread_join(worker_2, NULL) ;
			delwin(outer_win) ;
			delwin(input_win) ;
			endwin() ;
			exit(EXIT_SUCCESS) ;
		}

		if (strcmp(s, "continue") == 0) {
			sem_post(&ctx->sem) ;

			/*
			pthread_mutex_lock(&ctx->lock) ;
			ctx->paused = 0;
			pthread_cond_signal(&ctx->cond) ; 
			pthread_mutex_unlock(&ctx->lock)
			*/
		}

		if (strcmp(s, "paused") == 0) {
			sem_wait(&ctx->sem) ; 
		}

		/*TODO:
		 *
		 * create <text>  : create a new thread printing given text (ID should be automatically given)
		 * cancel <id>    : remove the thread identified by given ID
		 * stop <id>	  : pause the execution of thread of given ID
		 * continue <id>  : resume the execution of thread of given ID
		 */
	}



	return 0 ;

}
