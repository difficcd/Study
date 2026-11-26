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
#include <netinet/tcp.h>

#include "raylib.h"
#include "snake.h"

#define CELL_X     160   // 640 / 4
#define CELL_Y     100   // 400 / 4
#define MAX_ITEM   4
#define START_LEN  4
#define GAME_TIME  100
#define MAX_LEN    1024  // snake 최대 길이
#define SPEED      100  // tick 간격 (ms), 명세는 1000 = 1초

#ifndef MAXUSER
#define MAXUSER 4
#endif

int listen_fd;
int num_player = 2;

uint8_t user_count;
uint8_t color[MAXUSER] = {0, 1, 2, 3}; 

int conn_list[MAXUSER];
char name_list[MAXUSER][9];

Player * player[MAXUSER];           // 위치: cell 단위
Item item[MAX_ITEM];
Direction next_dir[MAXUSER];        // client가 보낸 방향
bool ate = false;

int game_started = 0;
int disconnected = 0;
uint8_t remaining;

pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER ;


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
		}  // 무한 루프 방지  (EOF)
		current_len += received_bytes;
	} // recv(conn, res, len, MSG_WAITALL); 
	return current_len;
}    

void constant_move(Player * player) {
    if      (player->direction == LEFT)   player->position.x -= 1;
    else if (player->direction == RIGHT)  player->position.x += 1;
    else if (player->direction == UP   )  player->position.y -= 1;
    else if (player->direction == DOWN )  player->position.y += 1;
}

int get_length(Player * player) {
    int length = 1;
    for(Body * i = player->next; i != NULL; i = i->next) length++;
    return length;
}

// 모든 snake 이동, touch->return 1 (set over)
int move_snakes ()
{
    for(int p = 0; p < user_count; p++){
        player[p]->direction = next_dir[p];

        // 기존 로직 (snake.c 이전)
        if(player[p]->next == NULL){ // 1개일 때
            for(int i = 0; i < MAX_ITEM; i++){
                if (player[p]->position.x == item[i].position.x && 
                    player[p]->position.y == item[i].position.y){ // 아이템 먹으면
                    item[i].position = (Vector2){ rand() % CELL_X, rand() % CELL_Y };
                    Body * new_body = calloc(1, sizeof(Body));
                    new_body->position = player[p]->position;
                    new_body->next = NULL;

                    player[p]->next = new_body;
                    player[p]->score += 1;
                    break;
                }
            }
            constant_move(player[p]);
        }
        else { // 2개 이상일 때
            /* player(앞으로 감) -> new_body(기존 player 정보) -> player가 가리키는 body */
            Body * new_body = calloc(1, sizeof(Body));
            new_body->position = player[p]->position;
            new_body->next = player[p]->next;
            player[p]->next = new_body;
            constant_move(player[p]);

            for(int i = 0; i < MAX_ITEM; i++){
                if (player[p]->position.x == item[i].position.x && 
                    player[p]->position.y == item[i].position.y){

                    if (get_length(player[p]) <= 255) ate = true; // length 가 1 byte 라 255 까지만 늘어남
                    item[i].position = (Vector2){ rand() % CELL_X, rand() % CELL_Y };

                    player[p]->score += 1;
                    printf("[Server] %s ate an item (score %d)\n", name_list[p], player[p]->score);
                    break;
                }
            }
            if(!ate){ // 아이템 안 먹었으면 꼬리 삭제
                Body * before_body = NULL;
                for(Body * i = player[p]->next; i != NULL; i = i->next){
                    if(i->next == NULL){
                        free(i);
                        before_body->next = NULL;
                        break;
                    }
                    before_body = i;
                }
            } // 먹었으면 삭제 안 함
            ate = false;
        }
    }

    // player가 필드 바깥 || player 에 닿으면 => 게임 종료
    int over = 0;
    for(int p = 0; p < user_count; p++){
        if (player[p]->position.x < 0 || player[p]->position.x >= CELL_X ||
            player[p]->position.y < 0 || player[p]->position.y >= CELL_Y)
        {
            printf("[Server] %s touch boundary\n", name_list[p]);
            over = 1;
        }

        for(int q = 0; q < user_count; q++){
            if(q == p) continue;

            if(player[p]->position.x == player[q]->position.x && 
               player[p]->position.y == player[q]->position.y) 
               over = 1; 

            for(Body * i = player[q]->next; i != NULL; i = i->next){
                if(player[p]->position.x == i->position.x && 
                   player[p]->position.y == i->position.y) over = 1;
            }
            if(over) printf("[Server] %s touch %s\n", name_list[p], name_list[q]);
        }
    }
    return over;
}

// UPDATE payload 만들기
void make_update (char * buf)
{
    int idx = 0;

    for(int p = 0; p < user_count; p++){
        buf[idx++] = color[p];
        buf[idx++] = get_length(player[p]);

        int count = 0;
        buf[idx + count * 2] = player[p]->position.x;   // head
        buf[idx + count * 2 + 1] = player[p]->position.y;
        count++;
        for(Body * i = player[p]->next; i != NULL; i = i->next){
            buf[idx + count * 2] = i->position.x;
            buf[idx + count * 2 + 1] = i->position.y;
            count++;
        }
        for( ; count < MAX_LEN; count++){        // 남는 칸은 0 (고정 크기)
            buf[idx + count * 2] = 0;
            buf[idx + count * 2 + 1] = 0;
        }
        idx += 2 * MAX_LEN;
    }
    for(int i = 0; i < MAX_ITEM; i++){
        buf[idx++] = item[i].position.x;
        buf[idx++] = item[i].position.y;
    }
    buf[idx++] = remaining;
}


void * game_runner (void * arg)
{
    char buf[5 + 2050 * MAXUSER + 9]; // UPDATE 기준으로 잡아둠
    uint32_t len;

    pthread_mutex_lock(&lock);
    // (1) initialization
    for(int p = 0; p < user_count; p++){
        player[p] = malloc(sizeof(Player));
        player[p]->position = (Vector2){ 13, 10 + 20 * p };
        player[p]->score  = 0;
        player[p]->next = NULL;
        player[p]->direction = RIGHT; // initial direction: RIGHT
        next_dir[p] = RIGHT;

        Body * tail = NULL;
        for (int i = 1; i < START_LEN; i++) { // 4칸시작
            Body * new_body = calloc(1, sizeof(Body));
            new_body->position = (Vector2){
                player[p]->position.x - i,
                player[p]->position.y
            };
            new_body->next = NULL;

            if (player[p]->next == NULL) 
                player[p]->next = new_body;
            else tail->next = new_body;
            tail = new_body;
        }
    }
    for(int i = 0; i < MAX_ITEM; i++){
        item[i].position = (Vector2){ rand() % CELL_X, rand() % CELL_Y };
    }
    remaining = GAME_TIME;
    disconnected = 0;

    // (2) START broadcast : (username 8 + color 1) * N
    buf[0] = START; // header : type 1 + length 4

    len = htonl(9 * user_count);
    memcpy(buf + 1, &len, 4);
    for(int p = 0; p < user_count; p++){
        memcpy(buf + 5 + (p * 9), name_list[p], 8);
        buf[5 + (p * 9) + 8] = color[p];
        // buf에 헤더1개, 인원수만큼 페이로드 붙여서 통째로 broadcast
        // 간단하게 정보 여러개를 묶어서 보낼 수 있음

        //     header(5B)           start payload(9B)
        // (type 1 + length 4)    (username 8 + color 1) 
    }
    for(int i = 0; i < user_count; i++)
        send_bytes(conn_list[i], buf, 5 + 9 * user_count);
    printf("[Server] game start\n");
    pthread_mutex_unlock(&lock);


    // (3) tick 기점으로 이동 + UPDATE broadcast
    int current_time = 0;   
    while(1) {
        usleep(SPEED * 1000);

        pthread_mutex_lock(&lock);
        current_time += SPEED;
        while(current_time >= 1000 && remaining > 0) { 
            remaining--; // 전역타이머 가감
            current_time -= 1000;
        }
        int over = move_snakes();

        buf[0] = UPDATE;
        len = htonl(2050 * user_count + 9);
        memcpy(buf + 1, &len, 4);

        make_update(buf + 5);
        for(int i = 0; i < user_count; i++)
            send_bytes(conn_list[i], buf, 5 + 2050 * user_count + 9);
        
        // 종료 : 
        // 정상종료조건(게임로직상:movesnake에서)    
        // 중도퇴장 (client runner의 recv로 세팅) 
        // 남은 시간 0 (타임어택)

        if(disconnected) {
            printf("[Server] player disconnected\n");
            over = 1;
        }
        if(remaining == 0) {
            printf("[Server] time over\n");
            over = 1;
        }
        pthread_mutex_unlock(&lock);

        if(over) break;
    }

    // (4) 결과 출력, TERMINATE  broadcast
    pthread_mutex_lock(&lock);

    int best = 0;
    for(int p = 0; p < user_count; p++){
        printf("[Server] %s : %d\n", name_list[p], player[p]->score);
        if(player[p]->score > best) best = player[p]->score;
    }
    for(int p = 0; p < user_count; p++)
        if(player[p]->score == best) printf("[Server] winner : %s\n", name_list[p]);
    

    buf[0] = TERMINATE;
    len = htonl(0);
    memcpy(buf + 1, &len, 4);
    for(int i = 0; i < user_count; i++)
        send_bytes(conn_list[i], buf, 5);
    printf("[Server] game end\n");

    // (5) 자원 정리
    game_started = 0;
    for(int p = 0; p < user_count; p++){
        shutdown(conn_list[p], SHUT_RDWR);  
        // 각 client thread 의 recv 가 끝나면서 close 하도록 RW shutdown
        Body * i = player[p]->next;       
        while(i != NULL){
            Body * next = i->next;
            free(i);
            i = next;
        }
        free(player[p]);
    }
    user_count = 0;

    pthread_mutex_unlock(&lock);
    return NULL;
}


void * client_service_runner (void * arg) 
{
    int conn_fd = *((int *)arg) ;
    free(arg) ;

    /*TO-DO(3): receive the signin message */
    char username[9] = {0};
    if(recv_bytes(conn_fd, username, 8) <= 0) {
        close(conn_fd);
        return NULL;
    }

    pthread_mutex_lock(&lock);
    if(game_started || user_count >= num_player) {
        pthread_mutex_unlock(&lock);

        char buf[5] = { ERROR, 0, 0, 0, 0 };   // length 0
        send_bytes(conn_fd, buf, 5);

        close(conn_fd);
        return NULL;
    }
    int me = user_count;

    // (1) color info send 
    char buf[6];
    uint32_t len = htonl(1);
    buf[0] = ACCEPT;
    memcpy(buf + 1, &len, 4);
    buf[5] = color[me];
    send_bytes(conn_fd, buf, 6);
    
    conn_list[me] = conn_fd;
    strcpy(name_list[me], username);
    printf("[Server] %s signs in (color %d)\n", username, color[me]);
    user_count ++;

    // (2) 다 모이면 게임시작
    if(num_player == user_count) {
        game_started = 1;
        pthread_t game_thread ;
        pthread_create(&game_thread, NULL, game_runner, NULL) ;
        pthread_detach(game_thread) ;
    }
    pthread_mutex_unlock(&lock);


    // (3) 방향키정보 수신 
    while(1) {
        uint8_t key ;
        if(recv_bytes(conn_fd, (char *) &key, 1) <= 0) break;

        pthread_mutex_lock(&lock);
        if(game_started) next_dir[me] = key;
        pthread_mutex_unlock(&lock);
    }

    // game 중도퇴장(recv EOF break) == 종료
    pthread_mutex_lock(&lock);
    if(game_started) disconnected = 1;
    pthread_mutex_unlock(&lock);

    close(conn_fd);
    return NULL;
}

void sig_handler (int signum) 
{
    /*TO-DO(7): Close all connections and terminate the program when SIGINT is called.*/
    if(signum != SIGINT || signum != SIGTERM || signum != SIGPIPE) 
        exit(EXIT_FAILURE);

    for(int i = 0; i < user_count; i++) 
        close(conn_list[i]);

    close(listen_fd);

    exit(EXIT_SUCCESS);

}


int main (int argc, char ** argv)
{
    char * host = strdup("127.0.0.1") ;
    int port = 0 ;

    user_count = 0;

    if(argc != 7) {
        printf("Usage: ./snaked -h <ip> -p <port> -n <num-players>\n");
        return 1;
    }

    signal(SIGINT,  sig_handler);
    signal(SIGTERM, sig_handler);
    signal(SIGPIPE, SIG_IGN); // exit 사용자로 인해 서버 죽는 것 방지..
    srand((unsigned int)time(NULL));


    int opt ;
    while ((opt = getopt(argc, argv, "p:h:n:")) != -1) {
        switch (opt) {
            case 'p':
                port = atoi(optarg) ;
                break;
            case 'h':
                free(host) ;
                host = strdup(optarg) ;
                break;
            case 'n':
                num_player = atoi(optarg) ;
                break;
            default:
                fprintf(stderr, "Unknown option: %s\n", optarg) ;
                exit(EXIT_FAILURE) ;
        }
    }

    if(num_player < 1 || num_player > MAXUSER) {
        printf("num_player : 1 ~ 4\n");
        return 1;
    }

    /*TO-DO(8): bind and listen*/
    listen_fd = socket(AF_INET, SOCK_STREAM, 0);

    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_port = htons(port);

    if (inet_aton(host, &(addr.sin_addr)) == 0) {
		fprintf(stderr, "inet_aton(127.0.0.1, addr.sin_addr) returns zero\n") ;
		exit(EXIT_FAILURE) ;
	}
	if (bind(listen_fd, (struct sockaddr *) &addr, sizeof(addr)) == -1) {
		perror("bind() fails") ;
		exit(EXIT_FAILURE) ;
	}
	if (listen(listen_fd, num_player) == -1) {
		perror("listen(fd,16) fails") ;
		exit(EXIT_FAILURE) ;
	}
    
    do {
        int conn_fd = accept(listen_fd, NULL, NULL);
        if (conn_fd == -1) {
            perror("accept");
            exit(EXIT_FAILURE);
        }

        int on = 1;
        setsockopt(conn_fd, IPPROTO_TCP, TCP_NODELAY, &on, sizeof(on));

        pthread_t client_service_thread ;
        int *arg = malloc(sizeof(int));
        *arg = conn_fd;
        pthread_create(&client_service_thread, NULL, client_service_runner, (void *)arg) ;
    } while (1) ;


    close(listen_fd);

    exit(EXIT_SUCCESS) ;
}