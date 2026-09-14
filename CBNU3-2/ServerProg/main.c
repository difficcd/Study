#include <ncurses.h>
#include <pthread.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

typedef struct{
	pthread_t thread;
	int tid;
	int flag; // 1이면 pause
	char input[64];
} WorkerInfo;

WorkerInfo workers[100];
WINDOW * outer_win ;
WINDOW * inner_win ;
WINDOW * input_win ;

void * worker_thread (void * arg)
{
	WorkerInfo* args = (WorkerInfo*)arg;
	while(1) {
		while(args->flag == 1) 
			usleep(100000);

		wprintw(inner_win, "(%d) %s\n", args->tid+1, args->input) ;
		wrefresh(inner_win) ;
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



	int num = 0;

	/* default worker 1 */
	for(int i=0; i<2; i++){
		workers[num].tid = num;
		workers[num].flag = 0;
		if(num == 0) strcpy(workers[num].input, "hi");
		if(num == 1) strcpy(workers[num].input, "hello");
		pthread_create(&workers[num].thread, NULL, worker_thread, &workers[num]);
		num++;
	}


	while (1) {
		char s[128] ; 
		mvwgetnstr(input_win, 1, 2, s, 127) ;
		wmove(input_win, 1,2) ;
		wclrtoeol(input_win) ;

		char *command = strtok(s, " "); 	  // <command> in
		char *in = strtok(NULL, " "); 		  // command  <in> 

		if(command == NULL) continue;

		if (strcmp(command, "quit") == 0) {
			for(int i=0; i<num; i++)
				pthread_cancel(workers[i].thread);
			
			for(int i=0; i<num; i++)
				pthread_join(workers[i].thread, NULL);
		
			delwin(outer_win) ;
			delwin(input_win) ;
			endwin() ;
			exit(EXIT_SUCCESS) ;
		}

		// create <text>  
		// : create a new thread printing given text (ID should be automatically given)
		if (strcmp(command,"create") == 0) {
			if(in == NULL) continue;
			workers[num].tid = num;
			workers[num].flag = 0;
			strcpy(workers[num].input, in); 
			pthread_create(&workers[num].thread, NULL, worker_thread, (void*)&workers[num]) ;

			num++; 
		} 

		// cancel <id>    : remove the thread identified by given ID
		if (strcmp(command,"cancel") == 0) {
			if(in == NULL) continue;
			int this_tid = atoi(in)-1;
			if(this_tid < 0 || this_tid >= num) continue; 

			int temp = pthread_cancel(workers[this_tid].thread);
			if(temp != 0) return EXIT_FAILURE ;
			temp = pthread_join(workers[this_tid].thread, NULL);
		}

		// stop <id>	  : pause the execution of thread of given ID
		if (strcmp(command,"stop") == 0) {
			if(in == NULL) continue;
			int this_tid = atoi(in)-1;
			if(this_tid < 0 || this_tid >= num) continue; 

			workers[this_tid].flag = 1;
		}

		// continue <id>  : resume the execution of thread of given ID
		if (strcmp(command,"continue") == 0) {
			if(in == NULL) continue;
			int this_tid = atoi(in)-1;
			if(this_tid < 0 || this_tid >= num) continue; 

			workers[this_tid].flag = 0;
		}

		// list(extended) : => print  paused tid(int) list  at input window 3sec
		if (strcmp(command,"stoplist") == 0) {
			for(int i=0; i<num; i++){
				if(workers[i].flag == 1){
					wprintw(input_win, "(%d)%s ", workers[i].tid+1, workers[i].input) ;
					wrefresh(input_win) ;
				}
			}
			sleep(2);
			wrefresh(input_win) ;
		}	
	}

	return 0 ;

}
