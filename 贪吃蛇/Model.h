#ifndef __MODEL_H__
#define __MODEL_H__

//坐标结构体
typedef struct Position
{
	int x;
	int y;
}Position;

//蛇前进方向枚举
typedef enum Direction
{
	UP,RIGHT,DOWN,LEFT
}Direction;

//链表结点的结构体
typedef struct Node
{
	Position pos;
	struct Node *next;
}Node;
//蛇本身
typedef struct Snake
{
	Node *head;
	Direction direction;
}Snake;
//游戏
typedef struct Game
{
	Snake snake;
	Position food;
	int score;
	int speed;
	int width;
	int height;
}Game;

void GameInit(Game *pGame);
void GameDestroy(Game *pGame);
void FoodInit(Position *pFood, int width, int height, Snake *pSnake);
void AddSnakeHead(Snake *pSnake, Position nextPos);
void RemoveSnakeTail(Snake *pSnake);

#endif //__MODEL_H__