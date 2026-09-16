#include <ncurses.h>
#include <pthread.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define TREADMAX 100

typedef struct {
	pthread_t thread;
	int flag; 
	int tid;  
	char input[100];

	pthread_mutex_t locks 
	// 스레드 개수 (workers[i].flag)
}WorkInfo;

WorkInfo workers[TREADMAX]; 

pthread_mutex_t winlock = PTHREAD_MUTEX_INITIALIZER;
WINDOW * outer_win ;
WINDOW * inner_win ;
WINDOW * input_win ;  


void * worker_thread (void * arg)
{
	WorkInfo* args = (WorkInfo*) arg;
	
	while(1){
		int paused = 0;
		pthread_mutex_lock(&args->locks);
		paused = args->flag;
		pthread_mutex_unlock(&args->locks);

		if (paused) { 
			usleep(100000); 
			continue; // while(1) 로 전이
		} 

		pthread_mutex_lock(&winlock);
		wprintw(inner_win, "(%d) %s\n",args->tid+1, args->input) ;
		wrefresh(inner_win);
		pthread_mutex_unlock(&winlock);

		sleep(1) ;
	}
}

int main(void)
{
	// window settings begin
		initscr();
		cbreak();
		curs_set(1);

		int rows, cols;
		getmaxyx(stdscr, rows, cols);

		int input_height = 3;
		int output_height = rows - input_height;

		outer_win = newwin(output_height, cols, 0, 0);
		inner_win = derwin(outer_win, output_height - 2, cols - 2, 1, 1) ;

		input_win = newwin(input_height, cols, output_height, 0);

		scrollok(inner_win, TRUE);

		box(outer_win, 0, 0);
		box(input_win, 0, 0);

		mvwprintw(outer_win, 0, 2, " Output ");
		mvwprintw(input_win, 0, 2, " Command ");

		wrefresh(outer_win);
		wrefresh(input_win);
	// window settings end

	int num = 0; //create standard 

	// flag mutex initialization 
	for (int i=0; i<TREADMAX; i++)
		pthread_mutex_init(&workers[i].locks, NULL);

	while (1) {
		char s[128] ;

		mvwgetnstr(input_win, 1, 2, s, 127) ; // input str
		wmove(input_win, 1,2) ;
		wclrtoeol(input_win) ;

		char* cmd = strtok(s, " ");
		char* in = strtok(NULL, " ");

		if(cmd == NULL) continue;

		if (strcmp(cmd, "quit") == 0) {
			for(int i=0; i<num; i++)
				pthread_cancel(workers[i].thread);
			for(int i=0; i<num; i++)
				pthread_join(workers[i].thread, NULL);

			delwin(outer_win) ;
			delwin(input_win) ;
			endwin() ;
			exit(EXIT_SUCCESS) ;
		}

		if (strcmp(cmd, "create") == 0) {
			if(in == NULL) continue;
			workers[num].flag = 0;
			workers[num].tid = num; 
			strcpy(workers[num].input, in);

			pthread_create(&workers[num].thread, NULL, worker_thread, (void*)&workers[num]);
			num++;
		}

		if (strcmp(cmd, "cancel") == 0) {
			if(in == NULL) continue;
			int this_tid = atoi(in)-1;
			if( this_tid<0 || this_tid >=num) continue; // out of boundry

			int temp = pthread_cancel(workers[this_tid].thread);
			if(temp == 0) pthread_join(workers[this_tid].thread, NULL);
		}

		if (strcmp(cmd, "stop") == 0) {
			if(in == NULL) continue;
			int this_tid = atoi(in)-1;
			if( this_tid<0 || this_tid >=num) continue; 

			pthread_mutex_lock(&workers[this_tid].locks);
			workers[this_tid].flag = 1; 
			pthread_mutex_unlock(&workers[this_tid].locks);
		}

		if (strcmp(cmd, "continue") == 0) {
			if(in == NULL) continue;
			int this_tid = atoi(in)-1;
			if( this_tid<0 || this_tid >=num) continue; 

			pthread_mutex_lock(&workers[this_tid].locks);
			workers[this_tid].flag = 0;
			pthread_mutex_unlock(&workers[this_tid].locks);
		}
	}

	// mutex cleanup
	for (int i=0; i<TREADMAX; i++)
		pthread_mutex_destroy(&workers[i].locks);

	return 0 ;
}
