#pragma warning(disable:4996)
#include "game.h"

void menu()
{
	printf("*********************************\n");
	printf("****          1.play         ****\n");
	printf("****          0.exit         ****\n");
	printf("*********************************\n");
}

void game()
{
	char MineBoard[ROWS][COLS];
	char PlayBoard[ROWS][COLS];
	InitBoard(MineBoard, ROWS, COLS, '0');
	InitBoard(PlayBoard, ROWS, COLS, '*');
	SetBoard(MineBoard, ROWS, COLS);
	PrintBoard(MineBoard, ROWS, COLS);
	PrintBoard(PlayBoard, ROWS, COLS);
	Minesweeper(MineBoard, PlayBoard, ROW, COL);
}

void test()
{
	srand((unsigned int)time(NULL));
	int input = 0;
	do
	{
		menu();
		printf("请选择：\n");
		scanf("%d", &input);
		switch (input)
		{
		case 1:
			game();
			break;
		case 0:
			printf("退出游戏\n");
			break;
		default:
			printf("请重新选择：\n");
			break;
		}
	} while (input);
}

int main()
{
	test();
	system("pause");
	return 0;
}