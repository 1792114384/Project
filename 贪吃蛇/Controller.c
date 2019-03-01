#include "Model.h"
#include "View.h"
#include <Windows.h>
#include <stdbool.h>
#include <stdio.h>
#include <time.h>

static void Pause()
{
	while (1)
	{
		Sleep(300);
		if (GetAsyncKeyState(VK_SPACE))
		{
			break;
		}
	}
}

static Position GetNextPosition(Snake *pSnake)
{
	Position nextPos = pSnake->head->pos;
	switch (pSnake->direction)
	{
	case UP:
		nextPos.y -= 1;
		break;
	case DOWN:
		nextPos.y += 1;
		break;
	case LEFT:
		nextPos.x -= 1;
		break;
	case RIGHT:
		nextPos.x += 1;
		break;
	}
	return nextPos;
}

static bool IsEaten(Position nextPos, Position food)
{
	return nextPos.x == food.x && nextPos.y == food.y;
}

static bool IsHitWall(Position nextPos, int width, int height)
{
	if (nextPos.x < 0)
	{
		return true;
	}
	if (nextPos.x >= width)
	{
		return true;
	}
	if (nextPos.y < 0)
	{
		return true;
	}
	if (nextPos.y >= height)
	{
		return true;
	}
	return false;
}

static bool IsHitSelf(Position nextPos, Snake *pSnake)
{
	for (Node *cur = pSnake->head->next; cur != NULL; cur = cur->next)
	{
		if (cur->pos.x == nextPos.x && cur->pos.y == nextPos.y)
		{
			return true;
		}
	}
	return false;
}

void test()
{
	Game game;
	GameInit(&game);
	ViewInit(game.width, game.height);
	DisplayWall(game.width, game.height);
	DisplaySnake(&game.snake);
	DisplayScore(game.score);
	SetCurPos(30 * 2 + 10, 7);
	printf("上下左右按键表示移动，SPACE表示暂停，F1加速");
	while (1)
	{
		if (GetAsyncKeyState(VK_UP) && game.snake.direction != DOWN)
		{
			game.snake.direction = UP;
		}
		else if (GetAsyncKeyState(VK_DOWN) && game.snake.direction != UP)
		{
			game.snake.direction = DOWN;
		}
		else if (GetAsyncKeyState(VK_LEFT) && game.snake.direction != RIGHT)
		{
			game.snake.direction = LEFT;
		}
		else if (GetAsyncKeyState(VK_RIGHT) && game.snake.direction != LEFT)
		{
			game.snake.direction = RIGHT;
		}
		else if (GetAsyncKeyState(VK_SPACE))
		{
			Pause();
		}
		else if (GetAsyncKeyState(VK_F1)) {
			game.speed = 100;
		}
		Position nextPos = GetNextPosition(&game.snake);
		if (IsEaten(nextPos, game.food))
		{
			AddSnakeHead(&game.snake, nextPos);
			game.score += 10;
			if (game.speed >= 100)
			{
				game.speed -= 20;
			}
			DisplayScore(game.score);
			FoodInit(&game.food, game.width, game.height, &game.snake);
		}
		else
		{
			RemoveSnakeTail(&game.snake);
			AddSnakeHead(&game.snake, nextPos);
		}
		if (IsHitWall(nextPos, game.width, game.height))
		{
			break;
		}
		if (IsHitSelf(nextPos, &game.snake))
		{
			break;
		}
		Sleep(game.speed);
	}
	GameDestroy(&game);
}

void Welcome()
{
	SetCurPos(40, 13);
	printf("欢迎来到贪吃蛇\n");
	SetCurPos(40, 14);
	system("pause");
	system("cls");
}

int main()
{
	srand((unsigned)time(NULL));
	Welcome();
	test();
	system("pause");
	return 0;
}