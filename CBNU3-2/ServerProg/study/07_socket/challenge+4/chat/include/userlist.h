struct user_node_t {
	uint8_t username[16] ;
	uint8_t password[16] ;
	int conn_fd ;
	
	// 0: activate, 1: inactive, 2: deactivated
	uint8_t status ;
	uint64_t timestamp ;

	struct user_node_t * next ;
	struct user_node_t * prev ;
} ;

typedef struct user_node_t user_node ;

user_node * add_user (user_node ** user_list, char * username, uint8_t * password, int conn_fd) ;
user_node * find_user (user_node ** user_list, char * username) ;
int delete_user (user_node ** user_list, char * username) ;
