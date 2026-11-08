#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "cache.h"

entry cache = { 0, 0, 0 } ;

int main (int argc, char ** argv)
{
	if (argc != 2) {
		fprintf(stderr, "Usage : %s <workload>\n", argv[0]) ;
		return EXIT_FAILURE ;
	}

	FILE * fp ; 
	if ((fp = fopen("cache", "r"))== NULL) {
		return EXIT_FAILURE ;
	}
	for (entry * last = &cache ; !feof(fp) ; ) {
		char url[256] ;
		char ip[256] ;

		fscanf(fp, "%256s %256s", url, ip) ;
		last = add_entry(last, url, ip) ;
	}
	fclose(fp) ;
	
	struct timespec begin ;
	clock_gettime(CLOCK_REALTIME, &begin) ;

	FILE * fw ;
	if (!(fw = fopen(argv[1], "r"))) 
		return EXIT_FAILURE ;
	
	while (feof(fw) == 0) {
		char opcode[16] ;
		char url[256] ;
		char ip[256] ;
		if (fscanf(fw, "%16s", opcode) != 1) 
			break ;

		do {
			if (strcmp(opcode, "update") == 0) {
				if (fscanf(fw, "%256s %256s", url, ip) != 2) {
					fprintf(stderr, "Fail to read\n") ;
					break ; 
				}
				entry * e = update_entry(&cache, url, ip) ;
				if (e != NULL) {
					printf("%s %s\n", e->url, e->ip) ;
				}
				break ;
			}
			if (strcmp(opcode, "lookup") == 0) {
				if (fscanf(fw, "%256s", url) != 1) {
					fprintf(stderr, "Fail to read\n") ;
					break ; 
				}
				entry * e = lookup_entry(&cache, url) ;
				if (e != NULL) {
					printf("%s %s\n", e->url, e->ip) ;
				}
			}

			fprintf(fw, "Invalid operation: %s\n", opcode) ;
		} while (0) ;
	}
	fclose(fw) ;	

	struct timespec curr ;
	clock_gettime(CLOCK_REALTIME, &curr) ;
	long int t = (curr.tv_sec - begin.tv_sec) * 1000000000 + (curr.tv_nsec - begin.tv_nsec) ;
	printf("%04ld.%09ld\n", t / 1000000000, t % 1000000000) ;

	return EXIT_SUCCESS ;
}
