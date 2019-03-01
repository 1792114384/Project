#ifndef __MODEL_H__
#define __MODEL_H__

//����ṹ��
typedef struct Position
{
	int x;
	int y;
}Position;

//��ǰ������ö��
typedef enum Direction
{
	UP,RIGHT,DOWN,LEFT
}Direction;

//������Ľṹ��
typedef struct Node
{
	Position pos;
	struct Node *next;
}Node;
//�߱���
typedef struct Snake
{
	Node *head;
	Direction direction;
}Snake;
//��Ϸ
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