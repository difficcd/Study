#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <time.h>
#include <stdint.h>

#include "userlist.h"

user_node * add_user (user_node ** user_list, char * username, uint8_t * password, int conn_fd) 
{
    if (*user_list) {
        for (user_node * i = *user_list ; i->next != NULL ; i = i->next) {
            if (strcmp(i->username, username) == 0) {
                return NULL ;
            }
        }
    }

	user_node * one_node = (user_node *) malloc(sizeof(user_node)) ;
	if (one_node == NULL) return NULL ;
	memcpy(one_node->username, username, 16) ;
	memcpy(one_node->password, password, 16) ;
	one_node->conn_fd = conn_fd ;
	one_node->next = NULL ;
	one_node->prev = NULL ;

    if (*user_list) {
        one_node->next = *user_list ;
        (*user_list)->prev = one_node ;
    }
    *user_list = one_node ;
	return one_node ;
}

user_node * find_user (user_node ** user_list, char * username)
{
    user_node * i = NULL ;
    for (i = *user_list ; i != NULL ; i = i->next) {
        if (strcmp((char *)i->username, username) == 0) {
            return i ;
        }
    }
    return NULL ;
}

int delete_user (user_node ** user_list, char * username){
    if (*user_list == NULL) return 1 ;

	user_node * i = find_user(user_list, username) ;
	if (i == NULL) return 1 ;

    if (i->prev != NULL) i->prev->next = i->next ;
    if (i->next != NULL) i->next->prev = i->prev ;

    if (*user_list == i) *user_list = i->next ;
    free(i) ;    

	return 0 ;
}

