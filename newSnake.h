#ifndef _NEW_SNAKE_H_
#define _NEW_SNAKE_H_

#define MAX_SANKE_LEN	100
#define FOOD_COUNT		3

#define MIN_SPEED		6000
#define MAX_SPEED		1000

#define ESC 			0x11b
#define PgUp			0x4900
#define PgDn			0x5100

#define KEY_UP			0x4800
#define KEY_DOWN		0x5000
#define KEY_LEFT		0x4b00
#define KEY_RIGHT		0x4d00

typedef struct DELTA_MOVE {
	int deltaCol;
	int deltaRow;
} DELTA_MOVE;

typedef struct SNAKE_BODY {
	int bodyCol;
	int bodyRow;
} SNAKE_BODY;

typedef struct SNAKE {
	int head;
	int len;
	int curLen;
	int maxLen;
	int direct;
	SNAKE_BODY snakeBody[MAX_SANKE_LEN];
} SNAKE;

typedef struct FOOD_POINT {
	int foodCol;
	int foodRow;
} FOOD_POINT;

void moveSnake(SNAKE *snake, int directionIndex);
void changeSnakeHeadPoint(SNAKE_BODY *newSnakeHeadPoint, SNAKE_BODY oldSnakeHeadPoint, int directionIndex);
void changeKey(int key, int *finished, int *directionIndex, int *initialSpeed);
void changeSpeed(int key, int *initialSpeed);
void makeFood(SNAKE snake, int foodCount, FOOD_POINT *foodPoint);
void swap(short *a, short *b);
void drowGameBorder();
void dataDisplay(int initialSpeed, int foodCount, SNAKE snake);
void storageBarrier(SNAKE snake);
void checkBarrier(SNAKE *snake, int *finished, int *foodCount, FOOD_POINT *foodPoint);

#endif