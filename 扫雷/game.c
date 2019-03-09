#pragma warning(disable:4996)
#include "game.h"

void InitBoard(char board[ROWS][COLS], int row, int col, char c)
{
	memset(board, c, sizeof(board[0][0]) * row * col);
}

void SetBoard(char board[ROWS][COLS], int row, int col)
{
	int x = 0;
	int y = 0;
	int i = 0;
	while (i < EASY_COUNT)
	{
		while (1)
		{
			x = rand() % 9 + 1;
			y = rand() % 9 + 1;
			if (board[x][y] == '0')
			{
				board[x][y] = '1';
				break;
			}
		}
		i++;
	}
}

void PrintBoard(char board[ROWS][COLS], int row, int col)
{
	for (int i = 1; i < row - 1; i++)
	{
		if (i == 1)
		{
			printf("  %d", i);
		}
		else
		{
			printf(" %d", i);
		}
	}
	printf("\n");
	for (int i = 1; i < row - 1; i++)
	{
		printf("%d ", i);
		for (int j = 1; j < col - 1; j++)
		{
			printf("%c ", board[i][j]);
		}
		printf("\n");
	}
	printf("\n");
}

int AroundCount(char board[ROWS][COLS], int x, int y)
{
	return board[x - 1][y] + board[x - 1][y - 1] + 
		   board[x][y - 1] + board[x + 1][y - 1] + 
		   board[x + 1][y] + board[x + 1][y + 1] + 
		   board[x][y + 1] + board[x - 1][y + 1] - 8 * '0';
}

int IfWin(char board[ROWS][COLS], int row, int col)
{
	int count = 0;
	for (int i = 1; i <= row; i++)
	{
		for (int j = 1; j <= col; j++)
		{
			if (board[i][j] == '*')
			{
				count++;
			}
		}
	}
	return count;
}

void FirstStep(char MineBoard[ROWS][COLS], char PlayBoard[ROWS][COLS], int row, int col, int x, int y)
{
		MineBoard[x][y] = '0';
		int i = 0;
		int j = 0;
		while (1)
		{
			i = rand() % 9 + 1;
			j = rand() % 9 + 1;
			if (MineBoard[i][j] == '0')
			{
				MineBoard[i][j] = '1';
				break;
			}
		}
		int ret = AroundCount(MineBoard, x, y);
		PlayBoard[x][y] = ret + '0';
		OpenBoard(MineBoard, PlayBoard, x, y);
		PrintBoard(PlayBoard, ROWS, COLS);
		PrintBoard(MineBoard, ROWS, COLS);
}

void Minesweeper(char MineBoard[ROWS][COLS], char PlayBoard[ROWS][COLS], int row, int col)
{
	int x = 0;
	int y = 0;
	int step = 1;
	while (1)
	{
		printf("请输入坐标:\n");
		scanf("%d%d", &x, &y);
		if (MineBoard[x][y] == '1' && step == 1)
		{
			FirstStep(MineBoard, PlayBoard, ROW, COL, x, y);
			step++;
			if (IfWin(PlayBoard, ROW, COL) == EASY_COUNT)
			{
				printf("恭喜你，YOU WIN!\n");
				PrintBoard(MineBoard, ROWS, COLS);
				break;
			}
			continue;
		}
		if (x >= 1 && x <= 9 && y >= 1 && y <= 9)
		{
			if (MineBoard[x][y] == '1')
			{
				printf("BOOM!你被炸飞了\n");
				PrintBoard(MineBoard, ROWS, COLS);
				break;
			}
			else
			{
				int ret = AroundCount(MineBoard, x, y);
				PlayBoard[x][y] = ret + '0';
				OpenBoard(MineBoard, PlayBoard, x, y);
				PrintBoard(PlayBoard, ROWS, COLS);
			}
		}
		else
		{
			printf("输入坐标非法，请重新输入\n");
		}
		if (IfWin(PlayBoard, ROW, COL) == EASY_COUNT)
		{
			printf("恭喜你，YOU WIN!\n");
			PrintBoard(MineBoard, ROWS, COLS);
			break;
		}
	}
}

void OpenBoard(char MineBoard[ROWS][COLS], char PlayBoard[ROWS][COLS], int x, int y)
{
	if (MineBoard[x][y] == '0' && PlayBoard[x][y] == '*')
	{
		PlayBoard[x][y] = AroundCount(MineBoard, x, y) + '0';
	}
	if (MineBoard[x - 1][y] == '0' && PlayBoard[x - 1][y] == '*')
	{
		PlayBoard[x - 1][y] = AroundCount(MineBoard, x - 1, y) + '0';
		if (AroundCount(MineBoard, x - 1, y) == 0)
		{
			OpenBoard(MineBoard, PlayBoard, x - 1, y);
		}
	}
	if (MineBoard[x - 1][y - 1] == '0' && PlayBoard[x - 1][y - 1] == '*')
	{
		PlayBoard[x - 1][y - 1] = AroundCount(MineBoard, x - 1, y - 1) + '0';
		if (AroundCount(MineBoard, x - 1, y - 1) == 0)
		{
			OpenBoard(MineBoard, PlayBoard, x - 1, y - 1);
		}
	}
	if (MineBoard[x][y - 1] == '0' && PlayBoard[x][y - 1] == '*')
	{
		PlayBoard[x][y - 1] = AroundCount(MineBoard, x, y - 1) + '0';
		if (AroundCount(MineBoard, x, y - 1) == 0)
		{
			OpenBoard(MineBoard, PlayBoard, x, y - 1);
		}
	}
	if (MineBoard[x + 1][y - 1] == '0' && PlayBoard[x + 1][y - 1] == '*')
	{
		PlayBoard[x + 1][y - 1] = AroundCount(MineBoard, x + 1, y - 1) + '0';
		if (AroundCount(MineBoard, x + 1, y - 1) == 0)
		{
			OpenBoard(MineBoard, PlayBoard, x + 1, y - 1);
		}
	}
	if (MineBoard[x + 1][y] == '0' && PlayBoard[x + 1][y] == '*')
	{
		PlayBoard[x + 1][y] = AroundCount(MineBoard, x + 1, y) + '0';
		if (AroundCount(MineBoard, x + 1, y) == 0)
		{
			OpenBoard(MineBoard, PlayBoard, x + 1, y);
		}
	}
	if (MineBoard[x + 1][y + 1] == '0' && PlayBoard[x + 1][y + 1] == '*')
	{
		PlayBoard[x + 1][y + 1] = AroundCount(MineBoard, x + 1, y + 1) + '0';
		if (AroundCount(MineBoard, x + 1, y + 1) == 0)
		{
			OpenBoard(MineBoard, PlayBoard, x + 1, y + 1);
		}
	}
	if (MineBoard[x][y + 1] == '0' && PlayBoard[x][y + 1] == '*')
	{
		PlayBoard[x][y + 1] = AroundCount(MineBoard, x, y + 1) + '0';
		if (AroundCount(MineBoard, x, y + 1) == 0)
		{
			OpenBoard(MineBoard, PlayBoard, x, y + 1);
		}
	}
	if (MineBoard[x - 1][y + 1] == '0' && PlayBoard[x - 1][y + 1] == '*')
	{
		PlayBoard[x - 1][y + 1] = AroundCount(MineBoard, x - 1, y + 1) + '0';
		if (AroundCount(MineBoard, x - 1, y + 1) == 0)
		{
			OpenBoard(MineBoard, PlayBoard, x - 1, y + 1);
		}
	}
}