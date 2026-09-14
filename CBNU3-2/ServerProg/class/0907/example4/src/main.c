#include <stdio.h>
#include <stdlib.h>
#include <ncurses.h>
#include <string.h>
#include "print_msg.h"

int main ()
{
	print_msg("Hello, world!", 3) ;

	char mesg[]="Hello, world" ;
	int row, col ;
	
	initscr();				/* start the curses mode */
	getmaxyx(stdscr,row,col);		/* get the number of rows and columns */
	
	mvprintw(row/2,(col-strlen(mesg))/2,"%s",mesg);
	/* print the message at the center of the screen */
	
	mvprintw(row-2,0,"This screen has %d rows and %d columns\n",row,col);
	printw("Try resizing your window(if possible) and then run this program again");
	
	refresh();
	getch();
	endwin();

}
