#include <stdio.h>
#include <conio.h>
#include <stdlib.h>
#include <time.h>

#include "newSnake.h"

const char *showSnakeHead = "^v<>";
const DELTA_MOVE deltaMove[4] = {{0, -1}, {0, 1}, {-1, 0}, {1, 0}};
short barrierArray[1625];

void checkBarrier(SNAKE *snake, int *finished, int *foodCount) {
	int col = snake->snakeBody[snake->head].bodyCol;
	int row = snake->snakeBody[snake->head].bodyRow;
	int snakeHeadElement = barrierArray[(row - 1) * 65 + col - 1];
	int i;

	if(snakeHeadElement == 1) {
		*finished = 1;
	}
	if(snakeHeadElement == 2) {
		*finished = 1;
	}
	if(snakeHeadElement == 3) {
		*finished = 1;
	}
	if(barrierArray[(row - 1) * 65 + col - 1] == 4) {
		(snake->len)++;
		--(*foodCount);
	}
}

/*把边界，障碍物，蛇身，食物的二维坐标转换为数组的一维下标存起来，分别为1， 2， 3， 4*/
void storageBarrier(SNAKE snake) {
	int i;
	int col;
	int row;
	int index;

	for(row = 1, col = 1; col <= 65; col++) {
		barrierArray[(row - 1) * 65 + col - 1] = 1;
	}
	for(row = 2, col = 1; row <= 25; row++) {
		barrierArray[(row - 1) * 65 + col - 1] = 1;
	}
	for(row = 25, col = 1; col <= 65; col++) {
		barrierArray[(row - 1) * 65 + col - 1] = 1;
	}
	for(row = 2, col = 65; row <= 25; row++) {
		barrierArray[(row - 1) * 65 + col - 1] = 1;
	}

	gotoxy(55, 8);
	putchar(219);
	gotoxy(16, 21);
	putchar(219);
	barrierArray[(8 - 1) * 65 + 55 - 1] = 2;
	barrierArray[(21 - 1) * 65 + 16 - 1] = 2;

	for(i = 0; i < snake.curLen; i++) {
		index = (snake.head - i + snake.maxLen) % snake.maxLen;
		col = snake.snakeBody[index].bodyCol;
		row = snake.snakeBody[index].bodyRow;
		barrierArray[(row - 1) * 65 + col - 1] = 3;
	}
}

/*显示右边的数据*/
void dataDisplay(int initialSpeed, int foodCount, SNAKE snake) {
	gotoxy(71, 2);
	printf("Speed");
	gotoxy(71, 3);
	printf("%4d", initialSpeed);

	gotoxy(69, 5);
	printf("foodCount");
	gotoxy(70, 6);
	printf("%4d", foodCount);

	gotoxy(67, 8);
	printf("snakeHeadPoint");
	gotoxy(71, 9);
	printf("%2d,%2d", snake.snakeBody[snake.head].bodyCol, snake.snakeBody[snake.head].bodyRow);

	gotoxy(68, 11);
	printf("snakeCurLen");
	gotoxy(70, 12);
	printf("%4d", snake.curLen);
}

/*画游戏边框*/
void drowGameBorder() {
	int col;
	int row;

	for(row = 1, col = 1; col <= 65; col++) {
		gotoxy(col, row);
		putchar(177);
	}
	for(row = 2, col = 1; row <= 25; row++) {
		gotoxy(col, row);
		putchar(177);
	}
	for(row = 25, col = 1; col <= 65; col++) {
		gotoxy(col, row);
		putchar(177);
	}
	for(row = 2, col = 65; row <= 25; row++) {
		gotoxy(col, row);
		putchar(177);
	}
}	

/*交换两个数*/
void swap(short *a, short *b) {
	short temp;

	temp = *a;
	*a = *b;
	*b = temp;
}

/*产生随机食物过程：
	1、定义一个能存储全屏幕坐标的一维数组，大小为80*25，初始化为0-1999
	2、根据蛇的当前长度，利用蛇头找到蛇的每个身体的坐标，把蛇身体坐标转换为一维数组的下标，此下标元素赋值为-1
	3、定义一个头指针，一个尾指针，分别从下标为0和1999相向遍历，若head对应的元素为-1，停下。若tail对应的元素不为-1，则停下
	4、交换head和tail对应的元素，就这样一直找，知道把所有的-1移到数组的最后
	5、在数组前面不为-1的元素里面，利用洗牌算法，取出一个随机数作为下标，把对应的元素再转换为坐标输出到屏幕上，即产生的食物*/
/*显示的区域：63列，23行*/
void makeFood(SNAKE snake, int foodCount, FOOD_POINT *foodPoint) {
	short screen[1449];
	int head = 0;
	int tail = 1448;
	int i;
	int y;
	int x;
	int index;
	int randIndex;
	int count = 1449;
	int foodCol;
	int foodRow;

	for(i = 0; i < 1449; i++) {
		screen[i] = i;
	}
	for(i = 0; i < snake.curLen; i++) {
		index = (snake.head - i + snake.maxLen) % snake.maxLen;
		y = snake.snakeBody[index].bodyCol;
		x = snake.snakeBody[index].bodyRow;
		screen[(x - 2) * 63 + y - 2] = -1;
		count--;
	}

	while(head <= tail) {
		while(screen[head] != -1) {
			head++;
		}
		while(screen[tail] == -1) {
			tail--;
		}
		if(head <= tail) {
			swap(&screen[head], &screen[tail]);
		}
	}

	srand(time(0));
	for(i = 0; i < foodCount; i++) {
		randIndex = rand() % ((count--) + 1);
		foodPoint[i].foodCol = screen[randIndex] % 63 + 2;
		foodPoint[i].foodRow = screen[randIndex] / 63 + 2;
		gotoxy(foodPoint[i].foodCol, foodPoint[i].foodRow);
		putchar('O');
		barrierArray[(foodPoint[i].foodRow - 1) * 65 + foodPoint[i].foodCol - 1] = 4;
		swap(&screen[randIndex], &screen[count]);
	}
}

/*根据按的键改变速度*/
void changeSpeed(int key, int *initialSpeed) {
	int deltaSpeed = 500;

	if(key == PgUp && *initialSpeed > MAX_SPEED) {
		*initialSpeed -= deltaSpeed;
	}
	if(key == PgDn && *initialSpeed < MIN_SPEED) {
		*initialSpeed += deltaSpeed;
	}
}

/*改变按键，执行对应按键的操作*/
void changeKey(int key, int *finished, int *directionIndex, int *initialSpeed) {
	if(key == ESC) {
		*finished = 1;
	} else if(key == KEY_UP) {
		*directionIndex = 0;
	} else if(key == KEY_DOWN) {
		*directionIndex = 1;
	} else if(key == KEY_LEFT) {
		*directionIndex = 2;
	} else if(key == KEY_RIGHT) {
		*directionIndex = 3;
	} else {
		changeSpeed(key, &(*initialSpeed));
	}
}

/*改变蛇头的坐标：用上一个蛇头的坐标加方向的增量*/
void changeSnakeHeadPoint(SNAKE_BODY *newSnakeHeadPoint, SNAKE_BODY oldSnakeHeadPoint, int directionIndex) {
	newSnakeHeadPoint->bodyCol = oldSnakeHeadPoint.bodyCol + deltaMove[directionIndex].deltaCol;
	newSnakeHeadPoint->bodyRow = oldSnakeHeadPoint.bodyRow + deltaMove[directionIndex].deltaRow;
}

/*移动蛇*/
/*
使用循环数组来存储蛇身体每个点的行，列坐标
初始显示一个蛇头，慢慢增多，知道等于设定的蛇的长度
蛇移动的过程：
	1、先在蛇头的位置输出一个蛇的身体
	2、再在之前蛇头的下一个位置输出一个蛇头
	3、消去尾部
*/
void moveSnake(SNAKE *snake, int directionIndex) {
	int oldSnakeHeadPoint;
	int removeOphiurid;
	SNAKE_BODY *snakeBody = snake->snakeBody;

	if(snake->curLen > 1) {
		gotoxy(snakeBody[snake->head].bodyCol, snakeBody[snake->head].bodyRow);
		putchar('*');
	}

	oldSnakeHeadPoint = snake->head;
	snake->head = (snake->head + 1) % snake->maxLen;
	changeSnakeHeadPoint(&snakeBody[snake->head], snakeBody[oldSnakeHeadPoint], directionIndex);
	gotoxy(snakeBody[snake->head].bodyCol, snakeBody[snake->head].bodyRow);
	putchar(showSnakeHead[directionIndex]);

	if(snake->curLen >= snake->len) {
		removeOphiurid = (snake->head - snake->len + snake->maxLen) % snake->maxLen;
		gotoxy(snakeBody[removeOphiurid].bodyCol, snakeBody[removeOphiurid].bodyRow);
		putchar(32);
		/*把上一次蛇尾在barrierArray中的数据清零*/
		barrierArray[(snakeBody[removeOphiurid].bodyRow - 1) * 65 + snakeBody[removeOphiurid].bodyCol - 1] = 0;	
	} else {
		(snake->curLen)++;
	}

	gotoxy(snakeBody[snake->head].bodyCol, snakeBody[snake->head].bodyRow);
}