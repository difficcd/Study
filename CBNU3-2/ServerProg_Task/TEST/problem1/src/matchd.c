#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>
#include <pthread.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netinet/in.h>

#include "match.h"

int send_bytes(int conn_fd, char * buf, int bytes) {
	ssize_t current_len = 0, temp_len = 0;
	while(current_len < bytes){
		temp_len = send(conn_fd, buf+current_len, bytes-current_len, 0);
		if(temp_len < 0) {
			perror("send_bytes");
			return -1;
		}
		current_len += temp_len;
	}
	return 0;
}

int recv_bytes(int conn_fd, char * buf, int bytes) {
	ssize_t current_len = 0, temp_len = 0;
	while(current_len < bytes){
		temp_len = recv(conn_fd, buf+current_len, bytes-current_len, 0);
		if(temp_len == 0) {
			return 1; // EOF
		}
		if(temp_len < 0) {
			perror("recv_bytes");
			return -1;
		}
		current_len += temp_len;
	}
	return 0;
}

/*TO-DO(1): define the data structures for teams, and the mutex and condition variables */
int user_count = 0;
int team_num = 1;
int last_out = 0;
pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t wait_team = PTHREAD_COND_INITIALIZER;
pthread_cond_t wait_in = PTHREAD_COND_INITIALIZER;

// 팀을 순차적으로 처리할 수밖에 없음, 팀결성자체를 병렬화는 불가능하고
// cli 들이 들어오는걸 병렬처리하되 3명모이면 name 들 가져다 징집해야하니까
// join_team(user1, msg) 하고 msg세팅을할때 지금 msg 에 달려있는 애들 이름 list 필요함
// user list 개념의 배열이 필요함 => 전역배열 쓰기

// 근데 왜 team number가 있는거냐??? team 여러개받는걸 상정한것??

char userlist[3][32] = {{0}};

/*TO-DO(2): wait until the team of this player is complete, and write the team message into "msg" */
void join_team (char * username, char * msg)
{
	// 3명 모이면 snprintf 해주는 로직 
	// 3명이 기다리고 있다는건 어떻게 ? => 지금까지 받은 username을 count
	// 전역변수로 count 하되 mutex 필요함
	// cond var를 통해 기다려야 하나? => 블로킹 하고 있다가 깨어나야 함. (busy wait 금지)

	pthread_mutex_lock(&lock);
	while(user_count == 3){
		pthread_cond_wait(&wait_in, &lock);
	}
	memcpy(userlist[user_count], username, 32);
	printf("userlist[%d]: %s\n", user_count, userlist[user_count]);
	user_count ++;
	pthread_cond_broadcast(&wait_team); // 들어갔으니까 signal뿌림

	if(user_count < 3) { // 내가 들어가고도 아직 부족하면 나도 기다려야함.
		while(!(user_count == 3)) {
			pthread_cond_wait(&wait_team, &lock);
		}
	}
	sprintf(msg, "team <%d>: <%s> <%s> <%s>\n", team_num, 
				   userlist[0], userlist[1], userlist[2]);
	last_out ++;

	if(last_out == 3) {
		// 한놈이 자원정리 역할 ..
		user_count = 0; // 세명 되어서 나갔으니까 다음애들한테 broadcast
		team_num ++;
		last_out = 0;
		pthread_cond_broadcast(&wait_in);
		pthread_cond_broadcast(&wait_team);
		pthread_mutex_unlock(&lock);
		return;
	}
	pthread_mutex_unlock(&lock);
}

void * client_service_runner (void * arg) 
{
	int conn_fd = *((int *)arg) ;
	free(arg) ;

	/*TO-DO(3): receive the username from the client */
	char username[32];
	if(recv_bytes(conn_fd, username, 32) == 1){
		printf("connection refused, %d out \n", conn_fd);
		return NULL;
	}
	printf("username: %s\n", username);

	char msg[MAX_MSG] = {0};
	/*TO-DO(4): join a team */
	join_team(username, msg);

	/*TO-DO(5): send the team message to the client, and close the connection */
	uint32_t msg_len = htonl(strlen(msg));
	send_bytes(conn_fd, (char *) &msg_len, 4);
	send_bytes(conn_fd, msg, strlen(msg));

	close(conn_fd);

	return NULL ;
}

/* TODO: Add more functions if needed */


int main (int argc, char ** argv)
{
	if(argc != 3) { 
		printf("Usage: ./bin/matchd -p <port>\n");
		exit(EXIT_FAILURE);
	}

	int port = 0 ;
	int opt ;
	while ((opt = getopt(argc, argv, "p:")) != -1) {
		switch (opt) {
			case 'p':
				port = atoi(optarg) ;
				break ;
			default:
				fprintf(stderr, "Invalid arguments\n") ;
				exit(EXIT_FAILURE) ;
		}
	}
	if (port == 0) {
		fprintf(stderr, "Invalid arguments\n") ;
		exit(EXIT_FAILURE) ;
	}

	int listen_fd ;
	/*TO-DO(6): bind and listen */
	listen_fd = socket(AF_INET, SOCK_STREAM, 0);

	struct sockaddr_in addr;
	addr.sin_family = AF_INET;
	addr.sin_port = htons(port);

	if(inet_aton("127.0.0.1", &addr.sin_addr) == 0) {
		fprintf(stderr, "inet_aton() fails");
		exit(EXIT_FAILURE);
	}
	if(bind(listen_fd, (struct sockaddr *) &addr, sizeof(addr))){
		perror("bind");
		exit(EXIT_FAILURE);
	}
	if(listen(listen_fd, 3)) {
		perror("listen");
		exit(EXIT_FAILURE);
	}


	do {
		/*TO-DO(7): accept a connection, and create a thread for it */
		pthread_t thread;
		int conn_fd = accept(listen_fd, NULL, NULL);
		printf("conn_fd : %d\n", conn_fd);

		int * fd = malloc(sizeof(int));
		*fd = conn_fd;
		pthread_create(&thread, NULL, client_service_runner, (void *)fd);
		pthread_detach(thread);

	} while (1) ;

	exit(EXIT_SUCCESS) ;
}
