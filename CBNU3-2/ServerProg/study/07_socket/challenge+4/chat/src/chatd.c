#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>
#include <endian.h>
#include <sys/stat.h>
#include <arpa/inet.h>

#include "userlist.h"
#include "chat.h"

/*TO-DO(1): Add a mutex for synchronzing accesses to user_list*/
user_node * user_list = NULL;

int broadcast (char * str, size_t len) 
{
    user_node * i ;

    for (i = user_list ; i != NULL ; i = i->next) {
        /*TO-DO(2): iterate over the user_list, and send "str" to every client*/
    
    }
    return 0 ;
}

void * client_service_runner (void * arg) 
{
    int conn_fd = *((int *)arg) ;
    free(arg) ;

    signin_msg signin ;
    /*TO-DO(3): receive the signin message */    

    user_node * user ;
    if ((user = find_user(&user_list, signin.username)) == NULL) {
        user = add_user(&user_list, signin.username, signin.password, conn_fd) ;
    }
    if (strcmp(user->password, signin.password) != 0) {
        printf("[%s] tried a sign in with a wrong password\n", user->username) ;
        /*TO-DO(4): send the FAIL response to the client*/
        close(conn_fd) ;
        return NULL ;
    }

    server_response response ;
    /*TO-DO(5): send the SUCCESS response to the client*/

    printf("[%s] successfully signed in.\n", user->username) ;

    char buf[512] ;
    sprintf(buf, "[Server] %s signs in.\n", user->username) ;
    broadcast(buf, strlen(buf)) ;

    do {
        upstream_msg message ;
        char content[256] ;

        /*TO-DO(6): receive a chat messsage from the corresponding client*/
       
        time_t tt = time(NULL) ;
        struct tm * ts = localtime(&tt) ;
        sprintf(buf, "%02d:%02d:%02d [%s] %s", 
                      ts->tm_hour, ts->tm_min, ts->tm_sec, user->username, content) ;
        broadcast(buf, strlen(buf)) ;
    } while (conn_fd > 0) ;

    delete_user(&user_list, user->username) ; // 지워도됨 (pw사용?)
    sprintf(buf, "[Server] %s signs off.\n", user->username) ;
    broadcast(buf, strlen(buf)) ;
    return NULL ;
}

int sig_handler (int signum) 
{
    /*TO-DO(7): Close all connections and terminate the program when SIGINT is called.*/
    // 모든 경우를 생각해야 함. broadcast 도중에 sig hand 올수도있고 ...
}

int main (int argc, char ** argv)
{
    char * host = strdup("127.0.0.1") ;
    int port = 0 ;

    int opt ;
    while ((opt = getopt(argc, argv, "p:h:")) != -1) {
        switch (opt) {
            case 'p':
                port = atoi(optarg) ;
                break;
            case 'h':
                free(host) ;
                host = strdup(optarg) ;
                break;
            default:
                fprintf(stderr, "Unknown option: %s\n", optarg) ;
                exit(EXIT_FAILURE) ;
        }
    }

    int listen_fd ;
    /*TO-DO(8): bind and listen*/

    do {
        int conn_fd = accept(listen_fd, NULL, NULL);
        if (conn_fd == -1) {
            perror("accept");
            exit(EXIT_FAILURE);
        }

        pthread_t client_service_thread ;
        int *arg = malloc(sizeof(int));
        *arg = conn_fd;
        pthread_create(&client_service_thread, NULL, client_service_runner, (void *)arg) ;
    } while (1) ;

    exit(EXIT_SUCCESS) ;
}