#include "View.h"
#include "Model.h"
#include <Windows.h>
#include <assert.h>
#include <stdio.h>

void SetCurPos(int x, int y)
{
	HANDLE hStdOutput = GetStdHandle(STD_OUTPUT_HANDLE);
	COORD coord = { x, y };
	SetConsoleCursorPosition(hStdOutput, coord);
}

void ViewInit(int width, int height)
{
	HANDLE hStdOutput = GetStdHandle(STD_OUTPUT_HANDLE);
	CONSOLE_CURSOR_INFO info;
	GetConsoleCursorInfo(hStdOutput, &info);
	info.bVisible = 0;
	SetConsoleCursorInfo(hStdOutput, &info);
}

void DisplayWall(int width, int height)
{
	SetCurPos(0, 0);
	for (int i = 0; i < width + 2; i++)
	{
		printf("จ~");
	}
	SetCurPos(0, height + 1);
	for (int i = 0; i < width + 2; i++)
	{
		printf("จ~");
	}
	for (int i = 0; i < height + 2; i++)
	{
		SetCurPos(0, i);
		printf("จ~");
	}
	for (int i = 0; i < height + 2; i++)
	{
		SetCurPos(2 * (width + 1), i);
		printf("จ~");
	}
}

void DisplaySnake(Snake *pSnake)
{
	assert(pSnake);
	for (Node *cur = pSnake->head; cur != NULL; cur = cur->next)
	{
		SetCurPos(2 * (cur->pos.x + 1), cur->pos.y + 1);
		if (cur == pSnake->head)
		{
			printf("กั");
		}
		else
		{
			printf("จ~");
		}
	}
}

void DisplayFood(int x, int y)
{
	SetCurPos(2 * (x + 1), y + 1);
	printf("จ~");
}

void DisplayScore(int score)
{
	SetCurPos(30 * 2 + 10, 10);
	printf("ตรทึ: %d", score);
}

void DisplaySnakeBlock(int x, int y)
{
	SetCurPos(2 * (x + 1), y + 1);
	printf("จ~");
}

void CleanSnakeBlock(int x, int y)
{
	SetCurPos(2 * (x + 1), y + 1);
	printf("  ");
}