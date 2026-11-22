#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <stdint.h>
#include "raylib.h"

#define CELL_SIZE    4
#define MAX_ITEM     4
#define FRAME        30 

#define WEIGHT    10    // 4 * '13' = 52, weight 은 cell크기와 반비례할 필요 있음
#define WIDTH     640
#define HEIGHT    400 
//#define OFFSET  52  // cell size의 배수이어야 함

#define STARTX    24
#define STARTY    24
// STARTX,Y도 cell 배수 

typedef enum GameScreen { LOGO, TITLE, GAMEPLAY, ENDING } GameScreen;
typedef enum Direction  {     UP, DOWN, RIGHT, LEFT     } Direction;

typedef struct Body{
	Vector2 position;
	struct Body * next;
} Body;

typedef struct Player {
    Vector2 position;    // head  
    Vector2 speed;
    Vector2 size;        // 고정이면 필요 없을 수 있음

    Direction direction; 
    Body * next;
    uint16_t score;  
} Player;

typedef struct Item {
    Vector2 position;
    Vector2 size;
} Item;


// int item_count = MAX_ITEM; 


void constant_move(Player * player) {
    if      (player->direction == LEFT)   player->position.x -= CELL_SIZE;
    else if (player->direction == RIGHT)  player->position.x += CELL_SIZE;
    else if (player->direction == UP   )  player->position.y -= CELL_SIZE;
    else if (player->direction == DOWN )  player->position.y += CELL_SIZE;
}

int main()
{
    int OFFSET = CELL_SIZE * WEIGHT ; // 4 * 13 = 52
    const int screenWidth  = WIDTH  + OFFSET * 2 ;
    const int screenHeight = HEIGHT + OFFSET * 2 ;

    SetConfigFlags(FLAG_WINDOW_HIGHDPI); // 노트북 환경을 위한 highdpi 설정
    InitWindow(screenWidth, screenHeight, "PROJECT: SNAKE GAME");
    
    GameScreen screen = LOGO;       // Current game screen state
    int framesCounter = 0;          // General pourpose frames counter
    int gameResult = -1;            // Game result: 0 - Loose, 1 - Win, -1 - Not defined
    bool gamePaused = false;        // Game paused state toggle
    

    Player * player = malloc(sizeof(Player));
    
    player->position = (Vector2){ OFFSET + STARTX, OFFSET + STARTY };
    player->speed = (Vector2){ 8.0f, 0.0f };
    player->size = (Vector2){ CELL_SIZE, CELL_SIZE };
    player->score  = 0;
    player->next = NULL;
    player->direction = RIGHT; // initial direction: RIGHT


    Item item[MAX_ITEM] = { 0 };
    srand((unsigned int)time(NULL));

    for(int i = 0; i < MAX_ITEM; i++){
        item[i].position = (Vector2){ 
            ((rand() % WIDTH  / CELL_SIZE) + 1) * CELL_SIZE - CELL_SIZE + OFFSET, 
            ((rand() % HEIGHT / CELL_SIZE) + 1) * CELL_SIZE - CELL_SIZE + OFFSET 
        };
		item[i].size = (Vector2){ CELL_SIZE, CELL_SIZE };
    }

    bool ate = false;
    char score[30];

    SetTargetFPS(FRAME);  // FPS: 30sec      
    
    while (!WindowShouldClose())    
    {
        // Update (MODEL)
        switch(screen) 
        {
            case LOGO: 
            {
                framesCounter++;
    
                if (framesCounter > 10) 
                {
                    screen = TITLE;    
                    framesCounter = 0;
                }
                
            } break;
            case TITLE: 
            {
                framesCounter++;

                if (IsKeyPressed(KEY_ENTER)) screen = GAMEPLAY;

            } break;
            case GAMEPLAY:
            { 
                if (!gamePaused)
                {
                    int ate_item_idx = -1; 

                    if (framesCounter % FRAME == 0) { // per sec
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

                    /*
                        if (IsKeyDown(KEY_RIGHT))       player.position.x += 4;
                        if (IsKeyDown(KEY_LEFT))   player.position.x -= 4;
                        else if (IsKeyDown(KEY_DOWN))   player.position.y += 4;
                        else if (IsKeyDown(KEY_UP))     player.position.y -= 4;
                    */
                }   

            } break;
            case ENDING: 
            {
           
                framesCounter++;
                /*if (IsKeyPressed(KEY_ENTER))
                {
                    screen = TITLE;
                }*/ 
                
            } break;
            default: break;
        }
       
        // VIEW
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
                    if ((framesCounter/30)%2 == 0) DrawText("PRESS [ENTER] to START", GetScreenWidth()/2 - MeasureText("PRESS [ENTER] to START", 20)/2, GetScreenHeight()/2 + 60, 20, DARKGRAY);
                    
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
                    
                    // if ((framesCounter/30)%2 == 0) DrawText("PRESS [ENTER] TO PLAY AGAIN", GetScreenWidth()/2 - MeasureText("PRESS [ENTER] TO PLAY AGAIN", 20)/2, GetScreenHeight()/2 + 80, 20, GRAY);
                    
                } break;
                default: break;
            }
        
        EndDrawing();
        
    }


    CloseWindow();           
    
    return 0;
}
