#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>
#include <endian.h>
#include <sys/stat.h>
#include <arpa/inet.h>
#include <time.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <signal.h>

#include "userlist.h"
#include "chat.h"

#ifndef MAXUSER
#define MAXUSER 16
#endif

/*TO-DO(1): Add a mutex for synchronzing accesses to user_list*/
pthread_mutex_t user_list_lock = PTHREAD_MUTEX_INITIALIZER;
volatile sig_atomic_t handler_on = 0;

user_node * user_list = NULL;
int listen_fd;

// send helper
ssize_t send_bytes (int conn, char *buf, size_t len) 
{
    int current_len = 0, sent_bytes = 0;
	while(current_len < len){
		sent_bytes = send(conn, buf+current_len, len-current_len, 0);
		// printf("debug sent bytes : %d\n", sent_bytes);
		if(sent_bytes < 0){
			// perror("string send: failed");
			return -1;
		}
		current_len += sent_bytes;
	}
	return current_len;
}

// recv helper 
ssize_t recv_bytes (int conn, char *buf, size_t len) 
{
    int current_len = 0, received_bytes = 0;
	while(current_len < len){
		received_bytes = recv(conn, buf+current_len, len-current_len, 0) ;

		if(received_bytes == -1){
			// perror("string receive: failed");
			return -1;
		}
		if(received_bytes == 0) {
            return 0;
		}  // 무한 루프 방지 
		current_len += received_bytes;
	} // recv(conn, res, len, MSG_WAITALL); 
	return current_len;
}    


int broadcast (char * str, size_t len, char * recv_name) // broadcast(buf, strlen(buf)) ;
{
    // broadcast upstream/signin message
    // signin message : "[Server] %s signs in.\n" (buf[512])

    user_node * i ;
    downstream_msg message;

    // 받은 시점이 아니라 보내주는 시점이라 오차 존재
    time_t current_time = time(NULL); 
    message.timestamp = htobe64((uint64_t)current_time);
    message.length = htobe64(len); 

    // message.username : 설계상 들어갈 수 없어 str에 세팅하여 전송

    if(recv_name[0] == '\0') message.flag = 0;
    else message.flag = 1;

    pthread_mutex_lock(&user_list_lock);
    int flag = 0;
    for (i = user_list ; i != NULL ; i = i->next) {
        /*TO-DO(2): iterate over the user_list, and send "str" to every client*/
        // if (i->conn_fd < 0) continue;
        
        if(message.flag == 1 && strcmp(recv_name, i->username) == 0) {
            // private send
            send_bytes(i->conn_fd, (char *) &message.flag,      1);
            send_bytes(i->conn_fd, (char *) &message.timestamp, 8);
            send_bytes(i->conn_fd, (char *) &message.length,    8);
            send_bytes(i->conn_fd, str, len); // payload (data)
            flag = 1;
            break;
        } 
        if (message.flag == 0) {
            // public send 
            send_bytes(i->conn_fd, (char *) &message.flag,      1);
            send_bytes(i->conn_fd, (char *) &message.timestamp, 8);
            send_bytes(i->conn_fd, (char *) &message.length,    8);
            send_bytes(i->conn_fd, str, len); // payload (data)
        }
        if (i->next == NULL && message.flag == 1 && flag == 0) {
            // If a recipient is not specified, the message is regarded as public (format:pri)
            // (not specified: i->next == NULL && message.flag == 1 && flag == 0)
            pthread_mutex_unlock(&user_list_lock);
            broadcast(str, len, "");
            return 0;
        }
    }
    pthread_mutex_unlock(&user_list_lock);

    return 0 ;
}


void * client_service_runner (void * arg) 
{
    int conn_fd = *((int *)arg) ;
    free(arg) ;

    /*TO-DO(3): receive the signin message */
    signin_msg signin ;    

    if(!recv_bytes(conn_fd, signin.username, 16)) {
        close(conn_fd);
        conn_fd = -1;
        return 0;
    }
	if(!recv_bytes(conn_fd, signin.password, 16)) {
        close(conn_fd);
        conn_fd = -1;
        return 0;
    }

    user_node * user ;

    
    int flag = 0 ; 

    pthread_mutex_lock(&user_list_lock);
    if ((user = find_user(&user_list, signin.username)) == NULL) {
        user = add_user(&user_list, signin.username, signin.password, conn_fd) ;
    }
    else if (strcmp(user->password, signin.password) != 0) flag = 1 ;
    else if (user->conn_fd > 0) flag = 2 ;
    else user->conn_fd = conn_fd ;
    pthread_mutex_unlock(&user_list_lock);

    if (flag != 0) {
        if (flag == 1)
            printf("[%s] tried a sign in with a wrong password\n", signin.username) ;
        if (flag == 2)
            printf("[%s] already signed in \n", signin.username) ;
        
        /*TO-DO(4): send the FAIL response to the client*/
        server_response response ;
        response.type = FAIL;
        uint32_t send_type;

        if (sizeof(server_response) == 4)  
            send_type = htonl(response.type);
        if (sizeof(server_response) == 2)  
            send_type = htons((uint16_t)response.type);
        // enum : 1B, 2B, 4B variable

        if(conn_fd > 0) 
            send_bytes(conn_fd, (char *)& send_type, sizeof(server_response));

        close(conn_fd) ;
        return NULL ;
    }


    /*TO-DO(5): send the SUCCESS response to the client*/
    server_response response ;
    response.type = SUCCESS;
    uint32_t send_type;

    if (sizeof(server_response) == 4)  
        send_type = htonl(response.type);
    if (sizeof(server_response) == 2)  
        send_type = htons((uint16_t)response.type);
    // enum : 1B, 2B, 4B variable

    if(conn_fd > 0)
        send_bytes(conn_fd, (char *)& send_type, sizeof(server_response));
    else {
        pthread_mutex_lock(&user_list_lock);
        user->conn_fd = -1;
        pthread_mutex_unlock(&user_list_lock);
    }

    // (1) signin : console print
    printf("[%s] successfully signed in.\n", user->username) ;

    // (2) signin : broadcast sign in message
    char buf[512] ;
    sprintf(buf, "[Server] %s signs in.\n", user->username) ;

    if(conn_fd > 0)
        broadcast(buf, strlen(buf), "") ;
    else user->conn_fd = -1;

    do {
        /*TO-DO(6): receive a chat messsage from the corresponding client*/

        upstream_msg message ;
        char content[256] ;     // 256? 1024?
        memset(content, 0, sizeof(content)); 
        memset(message.recipient, 0, 16);

        if(!recv_bytes(conn_fd, message.recipient, 16))
            break;

        uint64_t recv_msg_len ;
        if(!recv_bytes(conn_fd, (char *)& recv_msg_len, 8))
            break;      

        message.len = be64toh(recv_msg_len);
        if(message.len >= sizeof(content) || 
           !recv_bytes(conn_fd, content, message.len))
            break;      

        /* debug private/pub msg (recipient, content)
            if(message.recipient[0] != 0x0) 
                printf("\nthis is private msg!!");
            printf("\nadd debug: %s\n", content);
            printf("add debug: %s\n", message.recipient);
        */

        // timestamp: broadcast에 책임 넘기기 (downstream_msg 구조에 맞춤)

        // time_t tt = time(NULL) ;
        // struct tm * ts = localtime(&tt) ;
        // sprintf(buf, "%02d:%02d:%02d [%s] %s", 
        //              ts->tm_hour, ts->tm_min, ts->tm_sec, user->username, content) ;

        // @notspec content 는?
        if(message.recipient[0] != 0x0) { // private message
            sprintf(buf, "%s=>%s", user->username, content) ;
            //if(strcmp(user->username, message.recipient)) : broadcast 로직 변경 
                // broadcast(buf, strlen(buf), user->username, user->username) ;
        } 
        else // public message
            sprintf(buf, "[%s] %s", user->username, content) ;
 
        broadcast(buf, strlen(buf), message.recipient) ;

    } while (1) ;

    // delete_user(&user_list, user->username) ; 
    // printf("[Server] %s signs off.\n", user->username);
    
    // conn_fd 처리
    pthread_mutex_lock(&user_list_lock);
    if (user->conn_fd == conn_fd)   
        user->conn_fd = -1;
    pthread_mutex_unlock(&user_list_lock);
    close(conn_fd);

    sprintf(buf, "[Server] %s signs off.\n", user->username) ;
    broadcast(buf, strlen(buf), "") ;
    return NULL ;
}


void sig_handler (int signum) 
{
    // signum 확인 필요 ?? 
    // sigmask 허용 여부? nested signal ..
    // printf , lock 여지 안주는게좋음
    // 가능한 짧게, 메모리접근 위주로만 하는것이 정석

    // send helper 추가하듯 sendmsg 참조해서 써보기 (manpage)
    
    /*TO-DO(7): Close all connections and terminate the program when SIGINT is called.*/

    if (handler_on) return;   // nested signal 간단하게 방지
    handler_on = 1;

    if (signum == SIGINT || signum == SIGTERM) {
        user_node * i ;
        for (i = user_list ; i != NULL ; i = i->next) {
            close(i->conn_fd);
        }
        close(listen_fd);

        exit(0);
    }

}



int main (int argc, char ** argv)
{
    char * host = strdup("127.0.0.1") ;
    int port = 0 ;

    if(argc != 5) {
        printf("Usage: ./chatd -p <portnum> -h <host> \n");
        return 1;
    }

    signal(SIGINT,  sig_handler);
    signal(SIGTERM, sig_handler);
    signal(SIGPIPE, SIG_IGN); // exit 사용자로 인해 서버 죽는 것 방지..


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

    /*TO-DO(8): bind and listen*/
    listen_fd = socket(AF_INET, SOCK_STREAM, 0);

    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_port = htons(port);

    if (inet_aton("127.0.0.1", &(addr.sin_addr)) == 0) {
		fprintf(stderr, "inet_aton(127.0.0.1, addr.sin_addr) returns zero\n") ;
		exit(EXIT_FAILURE) ;
	}
	if (bind(listen_fd, (struct sockaddr *) &addr, sizeof(addr)) == -1) {
		perror("bind() fails") ;
		exit(EXIT_FAILURE) ;
	}
	if (listen(listen_fd, MAXUSER) == -1) {
		perror("listen(fd,16) fails") ;
		exit(EXIT_FAILURE) ;
	}
    
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


    close(listen_fd);

    exit(EXIT_SUCCESS) ;
}