#include <stdlib.h>
#include <string.h>
#include <stdio.h>

typedef struct entry_t {
		char * url ;
		char * ip ;
		struct entry_t * next ;
	}
	entry ;

entry * add_entry (entry * last, char * url, char * ip) 
{

	last->next = (entry *) malloc(sizeof(entry)) ;
	if (last->next == NULL) {
		fprintf(stderr, "Fail to allocate more memory\n") ;
		return NULL ;
	}
	last = last->next ;
	last->url = strdup(url) ;
	last->ip = strdup(ip) ;

}

entry * lookup_entry (entry * list, char * url) 
{
	for (entry * i = list->next ; i != NULL ; i = i->next) {
		int c = strcmp(i->url, url) ;
		if (c == 0) {
			return i ;
		}
	}
	return NULL ;
}

entry * update_entry (entry * list, char * url, char * ip)
{
	entry * prev = list ;
	for (entry * curr = prev->next ; curr != NULL ; prev = curr, curr = curr->next) {
		int c = strcmp(curr->url, url) ;
		if (c == 0) {
			free(curr->ip) ;
			curr->ip = strdup(ip) ;
			return curr ;
		}
	}

	entry * e ; 	
	if ((e = (entry *) malloc(sizeof(entry))) == NULL) 
		return NULL ;

	e->url = strdup(url) ;
	e->ip = strdup(ip) ;
	e->next = prev->next ;
	prev->next = e ;

	return e ;
}

void delete_entry (entry * list, char * url)
{
	entry * prev = list ;
	for (entry * curr = prev->next ; curr != NULL ; prev = curr, curr = curr->next) {
		int c = strcmp(curr->url, url) ;
		if (c == 0) {
			prev->next = curr->next ;
			free(curr->url) ;
			free(curr->ip) ;
			free(curr) ;
			break ;
		}
	}
}


