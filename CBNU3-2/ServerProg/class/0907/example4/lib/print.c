#include <stdio.h>

int print_msg (char * s, int n) 
{
	for (int i = 0 ; i < n ; i++) {
		printf("%s\n", s) ;
	}
	return 0 ;
}
