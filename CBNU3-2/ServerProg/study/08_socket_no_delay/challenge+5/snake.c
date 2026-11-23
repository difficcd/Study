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
#include <signal.h>
#include <unistd.h>
#include <pthread.h>

#include "raylib.h"
#include "snake.h"

#define CELL_SIZE    4
#define MAX_ITEM     4
#define FRAME        30
#define SPEED        10

#define WEIGHT    13    // 4 * '13' = 52, weight 은 cell크기와 반비례할 필요 있음
#define WIDTH     640
#define HEIGHT    400 
//#define OFFSET  52    // cell size의 배수이어야 함

#define STARTX    20
#define STARTY    20
// STARTX,Y도 cell 배수 



pthread_t downstream_thread ;
pthread_t upstream_thread ;


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


void constant_move(Player * player) {
    if      (player->direction == LEFT)   player->position.x -= CELL_SIZE;
    else if (player->direction == RIGHT)  player->position.x += CELL_SIZE;
    else if (player->direction == UP   )  player->position.y -= CELL_SIZE;
    else if (player->direction == DOWN )  player->position.y += CELL_SIZE;
}

void * upstream_thread_runner (void * arg)
{
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

                if (IsKeyPressed(KEY_ENTER)) screen = GAMEPLAY;

            } break;
            case GAMEPLAY:
            { 
                if (!gamePaused)
                {
                    int ate_item_idx = -1; 

                    if (framesCounter % (FRAME/SPEED) == 0) { // per sec

                        if(player->next == NULL){ // 1개일 때
                            for(int i = 0; i < MAX_ITEM; i++){
                                if (player->position.x == item[i].position.x && 
                                    player->position.y == item[i].position.y){ // 아이템 먹으면
                                    item[i].position = (Vector2){ 
                                        ((rand() % WIDTH  / CELL_SIZE) + 1) * CELL_SIZE - CELL_SIZE + OFFSET, 
                                        ((rand() % HEIGHT / CELL_SIZE) + 1) * CELL_SIZE - CELL_SIZE + OFFSET 
                                    };
                                    Body * new_body = calloc(1, sizeof(Body));
                                    new_body->position = player->position;
                                    new_body->next = NULL;

                                    player->next = new_body;
                                    player->score += 1;
                                    break;
                                }
                            }
                            constant_move(player);
                        }
                        else { // 2개 이상일 때
                            /* player(앞으로 감) -> new_body(기존 player 정보) -> player가 가리키는 body */
                            Body * new_body = calloc(1, sizeof(Body));
                            new_body->position = player->position;
                            new_body->next = player->next;
                            player->next = new_body;
                            constant_move(player);

                            for(int i = 0; i < MAX_ITEM; i++){
                                if (player->position.x == item[i].position.x && 
                                    player->position.y == item[i].position.y){

                                    ate = true;
                                    item[i].position = (Vector2){ 
                                        ((rand() % WIDTH  / CELL_SIZE) + 1) * CELL_SIZE - CELL_SIZE + OFFSET, 
                                        ((rand() % HEIGHT / CELL_SIZE) + 1) * CELL_SIZE - CELL_SIZE + OFFSET
                                    };

                                    player->score += 1;
                                    break;
                                }
                            }
                            if(!ate){ // 아이템 안 먹었으면 꼬리 삭제
                                Body * before_body = NULL;
                                for(Body * i = player->next; i != NULL; i = i->next){
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

                    // player가 필드 바깥으로 나갈 경우 게임 종료
                    if (player->position.x < OFFSET || player->position.x >= WIDTH  + OFFSET ||
                        player->position.y < OFFSET || player->position.y >= HEIGHT + OFFSET)
                    {
                        screen = ENDING;
                        break;
                    }

                    if (player->direction == UP || player->direction == DOWN){
                        if      (IsKeyDown(KEY_RIGHT))  player->direction = RIGHT;
                        else if (IsKeyDown(KEY_LEFT))   player->direction = LEFT;
                    }
                    else if (player->direction == LEFT || player->direction == RIGHT){
                        if      (IsKeyDown(KEY_UP)  )   player->direction = UP;
                        else if (IsKeyDown(KEY_DOWN))   player->direction = DOWN;
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
                    if ((framesCounter / 15 ) % 2 == 0) DrawText("PRESS [ENTER] to START", GetScreenWidth()/2 - MeasureText("PRESS [ENTER] to START", 20)/2, GetScreenHeight()/2 + 60, 20, DARKGRAY);
                    
                } break;
                case GAMEPLAY:
                { 
                    DrawRectangle(0, 0, WIDTH + (OFFSET * 2), HEIGHT + (OFFSET * 2), DARKGREEN); // Draw 필드 바깥 
                    DrawRectangle(OFFSET, OFFSET, WIDTH, HEIGHT, WHITE); // Draw 필드
                    sprintf(score, "score: %d", player->score);
                    DrawText(score, 16, 16, 20, WHITE); // 2사분면


                    DrawRectangle(player->position.x, player->position.y, player->size.x, player->size.y, BLACK);   
                    for(Body * i = player->next; i != NULL; i = i->next){
                        DrawRectangle(i->position.x, i->position.y, player->size.x, player->size.y, BLACK);   
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
		fprintf(stderr, "inet_aton(127.0.0.1, addr.sin_addr) returns zero\n") ;
		exit(EXIT_FAILURE) ;
	}
	if (connect(conn, (struct sockaddr *) &addr, sizeof(addr)) == -1) {
		perror("connect() fails");
		exit(EXIT_FAILURE);
	}

    // (1) color info recv 
    uint8_t color ;
    recv(conn, (char *) &color, 1, 0);
    printf("debug: %d\n", color);

    
    
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


    pthread_create(&upstream_thread, NULL, upstream_thread_runner, NULL) ;
	pthread_create(&downstream_thread, NULL, downstream_thread_runner, NULL) ;
	pthread_join(upstream_thread, NULL) ;
	pthread_join(downstream_thread, NULL) ;

    close(conn);
	exit(EXIT_SUCCESS) ;           
    
    return 0;
}
