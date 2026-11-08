typedef struct entry_t {
		char * url ;
		char * ip ;
		struct entry_t * next ;
	}
	entry ;
	
entry * add_entry    (entry * last, char * url, char * ip) ;
entry * lookup_entry (entry * list, char * url) ;
entry * update_entry (entry * list, char * url, char * ip) ;
void 	delete_entry (entry * list, char * url) ;
