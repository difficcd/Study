#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <string.h>
#include <stdint.h>
#include <endian.h>
#include <sys/stat.h>
#include <arpa/inet.h>
#include <time.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/tcp.h>
#include <signal.h>
#include <unistd.h>
#include <pthread.h>

#include "raylib.h"
#include "snake.h"

#define CELL_SIZE    4
#define MAX_ITEM     4
#define MAX_LEN      1024  // snake 최대 길이
#define FRAME        30

#define WEIGHT    13    // 4 * '13' = 52, weight 은 cell크기와 반비례할 필요 있음
#define WIDTH     640
#define HEIGHT    400 
//#define OFFSET  52    // cell size의 배수이어야 함

#define STARTX    20
#define STARTY    20
// STARTX,Y도 cell 배수 



pthread_t downstream_thread ;
pthread_t upstream_thread ;
pthread_t recipent_thread ;


// initialization 

int OFFSET ; // 4 * 13 = 52
int screenWidth  ;
int screenHeight ;

GameScreen screen = LOGO;       // Current game screen state
int framesCounter = 0;          // General pourpose frames counter
int gameResult = -1;            // Game result: 0 - Loose, 1 - Win, -1 - Not defined
bool gamePaused = false;        // Game paused state toggle


Player * player;
Item item[MAX_ITEM] = { 0 };
bool ate = false;
char score[30];

bool Window_ShouldClose = false;
pthread_mutex_t window_lock = PTHREAD_MUTEX_INITIALIZER ; 

// server 에서 받은 game 상태
uint8_t this_color;
int num_player = 0;
int remaining = 100;

char player_name[4][9];

int snake_len[4] = { 0 };
Vector2 snake_pos[4][MAX_LEN];     // pixel 위치, [0] : head

Color color_list[4] = { BLACK, BLUE, ORANGE, PURPLE };


// send helper
ssize_t send_bytes (int conn, char *buf, size_t len) 
{
    int current_len = 0, sent_bytes = 0;
	while(current_len < len){
		sent_bytes = send(conn, buf+current_len, len-current_len, 0);
		if(sent_bytes <= 0){
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
			return -1;
		}
		if(received_bytes == 0) {
            return 0;
		}  // 무한 루프 방지 
		current_len += received_bytes;
	}
	return current_len;
}    


void constant_move(Player * player) {
    if      (player->direction == LEFT)   player->position.x -= CELL_SIZE;
    else if (player->direction == RIGHT)  player->position.x += CELL_SIZE;
    else if (player->direction == UP   )  player->position.y -= CELL_SIZE;
    else if (player->direction == DOWN )  player->position.y += CELL_SIZE;
}

void * upstream_thread_runner (void * arg)
{
    int conn = *((int *)arg);

    while (1){
        pthread_mutex_lock(&window_lock);
        if(Window_ShouldClose == true) {
            pthread_mutex_unlock(&window_lock);
            break;
        }
        pthread_mutex_unlock(&window_lock);

        // SetTargetFPS 불가능 : usleep 으로 1초단위 맞춤
        // (frameCounter++ + usleep)

        switch(screen) 
        {
            case LOGO: 
            {
                framesCounter++;
                usleep(1000000 / FRAME); 

                if (framesCounter > 10) 
                {
                    screen = TITLE;    
                    framesCounter = 0;
                }
                
            } break;
            case TITLE: 
            {
                framesCounter++;
                usleep(1000000 / FRAME); 

                // server 가 START 를 보내면 recipent thread 가 GAMEPLAY 로 바꿈

            } break;
            case GAMEPLAY:
            { 
                if (!gamePaused)
                {
                    // 이동, 아이템, 충돌 => server 책임으로 옮김
                    Direction before = player->direction;

                    if (player->direction == UP || player->direction == DOWN){
                        if      (IsKeyDown(KEY_RIGHT))  player->direction = RIGHT;
                        else if (IsKeyDown(KEY_LEFT))   player->direction = LEFT;
                    }
                    else if (player->direction == LEFT || player->direction == RIGHT){
                        if      (IsKeyDown(KEY_UP)  )   player->direction = UP;
                        else if (IsKeyDown(KEY_DOWN))   player->direction = DOWN;
                    }

                    // 방향 1B 전송
                    if (player->direction != before) {
                        uint8_t key = player->direction;
                        send_bytes(conn, (char *) &key, 1);
                    }

                    framesCounter++;
                    usleep(1000000 / FRAME); 
                    // SetTargetFPS와 속도 맞춤
                }   

            } break;
            case ENDING: 
            {
                framesCounter++;
                usleep(1000000 / FRAME); 

            } break;
            default: break;
        }
    }
    return NULL;
}

void * recipent_thread_runner (void * arg)
{
    int conn = *((int *)arg);

    while (1) {
        
        // header : type 1B + payload len 4B
        uint8_t msg_type;
        uint32_t recv_len;
        if(recv_bytes(conn, (char *) &msg_type, 1) <= 0) break;
        if(recv_bytes(conn, (char *) &recv_len, 4) <= 0) break;
        uint32_t len = ntohl(recv_len);

        char * buf = calloc(len + 1, 1);
        if(recv_bytes(conn, buf, len) <= 0 && len > 0) break;

        if (msg_type == START) { // (username 8, color 1) * N
            num_player = len / 9;
            for(int p = 0; p < num_player; p++){
                memcpy(player_name[p], buf + p * 9, 8);
                player_name[p][8] = '\0';
            }
            screen = GAMEPLAY;
            // 묶어서 보낸것 p 순회하며 정보 담아줌 (list의 모든 name set)
        }
        else if (msg_type == UPDATE) { // snake * N + item * 4 + time
            int idx = 0;
            for(int p = 0; p < num_player; p++){
                idx++; // buf[0] : color..                                 
                snake_len[p] = (uint8_t) buf[idx++]; //len 1B
                for(int i = 0; i < MAX_LEN; i++){      
                    snake_pos[p][i] = (Vector2){ OFFSET + (uint8_t) buf[idx] * CELL_SIZE, 
                                                 OFFSET + (uint8_t) buf[idx + 1] * CELL_SIZE };
                    idx += 2; 
                    // buf로 (grid index) 를 보내주니까 그걸 좌표로 변환해야 함
                    // idx 는 좌표하나당 2칸이동 (x,y)
                } 
            }
            for(int i = 0; i < MAX_ITEM; i++){
                item[i].position = (Vector2){ OFFSET + (uint8_t) buf[idx] * CELL_SIZE, 
                                              OFFSET + (uint8_t) buf[idx + 1] * CELL_SIZE };
                idx += 2;
            } // 아이템도 이동할때마다 체크업해서 위치갱신
            remaining = (uint8_t) buf[idx];
            player->score = snake_len[this_color] - 4;
        }
        else if (msg_type == TERMINATE) {
            free(buf);
            break;
        }
        free(buf);
    }

    // game end || server connection refused 
    // recv EOF, TERMINATE
    screen = ENDING;
    return NULL;
}

void * downstream_thread_runner (void * arg)
{
    screenWidth  = WIDTH  + OFFSET * 2 ;
    screenHeight = HEIGHT + OFFSET * 2 ;

    SetConfigFlags(FLAG_WINDOW_HIGHDPI); // 노트북 환경을 위한 highdpi 설정
    InitWindow(screenWidth, screenHeight, "PROJECT: SNAKE GAME");

    // 검은 화면 원인 : InitWindow 와 같은 스레드에서 drawing 해야함
    
    SetTargetFPS(FRAME);  // FPS: 30sec    

    while (!WindowShouldClose()){
        BeginDrawing();
            ClearBackground(RAYWHITE);
            
            switch(screen) 
            {
                case LOGO: 
                {   
                    DrawText("LOGO SCREEN", 20, 20, 40, LIGHTGRAY);
                    
                } break;
                case TITLE: 
                {
                    DrawText("TITLE SCREEN", 20, 20, 40, DARKGREEN);
                    if ((framesCounter / 15 ) % 2 == 0) DrawText("WAITING FOR PLAYERS...", GetScreenWidth()/2 - MeasureText("WAITING FOR PLAYERS...", 20)/2, GetScreenHeight()/2 + 60, 20, DARKGRAY);
                    
                } break;
                case GAMEPLAY:
                { 
                    DrawRectangle(0, 0, WIDTH + (OFFSET * 2), HEIGHT + (OFFSET * 2), DARKGREEN); // Draw 필드 바깥 
                    DrawRectangle(OFFSET, OFFSET, WIDTH, HEIGHT, WHITE); // Draw 필드
                    sprintf(score, "score: %d", player->score);
                    DrawText(score, 16, 16, 20, WHITE); // 2사분면

                    // time 추가
                    sprintf(score, "time: %d", remaining);
                    DrawText(score, 16, HEIGHT + OFFSET + 16, 20, WHITE); // 3사분면

                    // name: 점수(==뱀길이-4:초기값)
                    for(int p = 0; p < num_player; p++){
                        sprintf(score, "%s: %d", player_name[p], snake_len[p] - 4);
                        DrawText(score, 200 + p * 130, 16, 20, color_list[p]);
                    }

                    // draw snake
                    for(int p = 0; p < num_player; p++){
                        for(int i = 0; i < snake_len[p]; i++){
                            DrawRectangle(snake_pos[p][i].x, snake_pos[p][i].y, player->size.x, player->size.y, color_list[p]);   
                        }
                    }

                    // draw item
                    for(int i = 0; i < MAX_ITEM; i++){
                        DrawRectangle(item[i].position.x, item[i].position.y, item[i].size.x, item[i].size.y, RED); 
                    }

                    if (gamePaused) DrawText("GAME PAUSED", screenWidth/2 - MeasureText("GAME PAUSED", 40)/2, screenHeight/2 + 60, 40, GRAY);
                    
                } break;
                case ENDING: 
                {    
                    DrawText("ENDING SCREEN", 20, 20, 40, DARKBLUE);

                    // 최종점수 출력 
                    for(int p = 0; p < num_player; p++){
                        sprintf(score, "%s: %d", player_name[p], snake_len[p] - 4);
                        DrawText(score, 20, 90 + p * 30, 20, color_list[p]);
                    }

                    // winner 출력 (best 기작이라 동점시 여러명 리스트업됨.)
                    int best = 0;
                    for(int p = 0; p < num_player; p++){
                        if(snake_len[p] > best) best = snake_len[p];
                    }
                    int y = 250;
                    for(int p = 0; p < num_player; p++){
                        if(snake_len[p] == best){
                            sprintf(score, "winner: %s", player_name[p]);
                            DrawText(score, 20, y, 30, color_list[p]);
                            y += 40; // 동점 대비
                        }
                    }
                } break;
                default: break;
            }
        EndDrawing();
    }

    pthread_mutex_lock(&window_lock);
    Window_ShouldClose = true;
    pthread_mutex_unlock(&window_lock);

    CloseWindow();

    return NULL;
}

int main(int argc, char ** argv)
{

    char username[9] = "" ;
    char server_ip[32] = "" ;
    int port = 0 ;

    if(argc != 7) {
        printf("Usage: ./snake -u <username> -h <server-ip> -p <port>\n");
        return 1;
    }
	
	int opt ;
	while ((opt = getopt(argc, argv, "u:h:p:")) != -1) {
		switch (opt) {
			case 'u':
				strncpy(username, optarg, 8) ;
				break ;
			case 'h':
				strncpy(server_ip, optarg, 32) ;
				break ;
			case 'p':
				port = atoi(optarg) ;
				break ;
			default:
				fprintf(stderr, "Invalid arguments\n") ;
				exit(EXIT_FAILURE) ;
		}		
	}

    /* TO-DO: connection */
	int conn = socket(AF_INET, SOCK_STREAM, 0);

    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_port = htons(port);

	if (inet_aton(server_ip, &(addr.sin_addr)) == 0) {
		fprintf(stderr, "inet_aton(server_ip, addr.sin_addr) returns zero\n") ;
		exit(EXIT_FAILURE) ;
	}
	if (connect(conn, (struct sockaddr *) &addr, sizeof(addr)) == -1) {
		perror("connect() fails");
		exit(EXIT_FAILURE);
	}

    int on = 1;
    setsockopt(conn, IPPROTO_TCP, TCP_NODELAY, &on, sizeof(on));

    // (1) username send (8B 만 보내고 초기화한 9번째칸 남겨둠)
    send_bytes(conn, username, 8);

    // (2) color info recv : header 5(type1B+payload4B) + color 1B
    char buf[6];
    if(recv_bytes(conn, buf, 6) <= 0 || buf[0] != ACCEPT) {
        printf("error: can't enter\n");
        exit(EXIT_FAILURE);
    }
    this_color = buf[5]; // color 1B 세팅
    printf("debug: %d\n", this_color);

    
    
    OFFSET = CELL_SIZE * WEIGHT ; // 4 * 13 = 52

    player = malloc(sizeof(Player));
    
    player->position = (Vector2){ OFFSET + STARTX, OFFSET + STARTY };
    player->speed = (Vector2){ 8.0f, 0.0f };
    player->size = (Vector2){ CELL_SIZE, CELL_SIZE };
    player->score  = 0;
    player->next = NULL;
    player->direction = RIGHT; // initial direction: RIGHT

    Body * tail = NULL;
    for (int i = 1; i < 4; i++) {
        Body * new_body = calloc(1, sizeof(Body));
        new_body->position = (Vector2){
            player->position.x - CELL_SIZE * i,
            player->position.y
        };
        new_body->next = NULL;

        if (player->next == NULL) 
            player->next = new_body;
        else tail->next = new_body;
        tail = new_body;
    }

    srand((unsigned int)time(NULL));
    for(int i = 0; i < MAX_ITEM; i++){
        item[i].position = (Vector2){ 
            ((rand() % WIDTH  / CELL_SIZE) + 1) * CELL_SIZE - CELL_SIZE + OFFSET, 
            ((rand() % HEIGHT / CELL_SIZE) + 1) * CELL_SIZE - CELL_SIZE + OFFSET 
        };
		item[i].size = (Vector2){ CELL_SIZE, CELL_SIZE };
    }  


    pthread_create(&recipent_thread, NULL, recipent_thread_runner, &conn) ; // 
    pthread_create(&upstream_thread, NULL, upstream_thread_runner, &conn) ;
	pthread_create(&downstream_thread, NULL, downstream_thread_runner, NULL) ;
	pthread_join(upstream_thread, NULL) ;
	pthread_join(downstream_thread, NULL) ;

    close(conn);
	exit(EXIT_SUCCESS) ;           
    
    return 0;
}
