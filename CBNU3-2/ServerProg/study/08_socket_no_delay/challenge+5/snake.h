#include <stdint.h>


typedef enum GameScreen { LOGO, TITLE, GAMEPLAY, ENDING } GameScreen;
typedef enum Direction  {     UP, DOWN, RIGHT, LEFT     } Direction;

typedef enum { ACCEPT, START, UPDATE, TERMINATE, ERROR } type ;

// game struct

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


// socket struct

typedef 
	struct {
		type header_type;
		uint32_t length ;
	}
	header ;

typedef 
	struct {
		uint64_t timestamp ;
		uint8_t  username[16] ;
		uint64_t length ;
		uint8_t flag;
	}
	downstream_msg ;

    /*
typedef
	struct {
		response_type type ;
	}
	server_response ;
    */