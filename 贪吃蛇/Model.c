#include "Model.h"
#include "View.h"
#include <assert.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

void SnakeInit(Snake *pSnake)
{
	assert(pSnake);
	pSnake->head = NULL;
	pSnake->direction = RIGHT;
	for (int i = 0; i < 3; i++)
	{
		Node *newNode = (Node *)malloc(sizeof(Node));
		newNode->pos.x = 3 + i;
		newNode->pos.y = 3;
		newNode->next = pSnake->head;
		pSnake->head = newNode;
	}
}

bool IsOverSnake(Snake *pSnake, int x, int y)
{
	for (Node *cur = pSnake->head; cur != NULL; cur = cur->next)
	{
		if (cur->pos.x == y && cur->pos.y == y)
		{
			return true;
		}
	}
	return false;
}

void FoodInit(Position *pFood, int width, int height, Snake *pSnake)
{
	assert(pFood);
	assert(pSnake);
	int x = 0;
	int y = 0;
	do
	{
		x = rand() % width;
		y = rand() % height;
	} while (IsOverSnake(pSnake, x, y));
	pFood->x = x;
	pFood->y = y;
	DisplayFood(x, y);
}

void GameInit(Game *pGame)
{
	assert(pGame);
	pGame->score = 0;
	pGame->width = 28;
	pGame->height = 27;
	pGame->speed = 300;
	SnakeInit(&pGame->snake);
	FoodInit(&pGame->food, pGame->width, pGame->height, &pGame->snake);
}

void SnakeDestroy(Snake *pSnake)
{
	assert(pSnake);
	Node *cur, *next;
	for (cur = pSnake->head; cur != NULL; cur = next)
	{
		next = cur->next;
		free(cur);
	}
}

void GameDestroy(Game *pGame)
{
	assert(pGame);
	SnakeDestroy(&pGame->snake);
}


void AddSnakeHead(Snake *pSnake, Position nextPos)
{
	assert(pSnake);
	Node *newNode = (Node *)malloc(sizeof(Node));
	newNode->pos.x = nextPos.x;
	newNode->pos.y = nextPos.y;
	newNode->next = pSnake->head;
	pSnake->head = newNode;
	DisplaySnake(pSnake);
}

void RemoveSnakeTail(Snake *pSnake)
{
	assert(pSnake);
	Node *cur;
	for (cur = pSnake->head; cur->next->next != NULL; cur = cur->next)
	{
		;
	}
	CleanSnakeBlock(cur->next->pos.x, cur->next->pos.y);
	free(cur->next);
	cur->next = NULL;
}