#include <stdlib.h>
#include <time.h>
#include <stdio.h>
#include <Windows.h>
#include <conio.h>

#define ROW 4
#define COL 4
//����ȫ�ֱ����ĺô����ں���֮�䲻�ô���
//              ȱ�㣺ֻ�����һ����Ϸ��Ҫ�������ͱ��뽫��������Ϊȫ0��
int arr[ROW][COL] = { 0 };
int score = 0;

//���ù��ľ����ͨ�׵�˵���������ù���λ��
void SetCurPos(int x, int y)
{
	HANDLE hStdOutput = GetStdHandle(STD_OUTPUT_HANDLE);
	COORD coord = { x, y };
	SetConsoleCursorPosition(hStdOutput, coord);
}
//���ع��
void ViewInit(int width, int height)
{
	HANDLE hStdOutput = GetStdHandle(STD_OUTPUT_HANDLE);
	CONSOLE_CURSOR_INFO info;
	GetConsoleCursorInfo(hStdOutput, &info);
	info.bVisible = 0;
	SetConsoleCursorInfo(hStdOutput, &info);
}

 //��ӡ
void GamePrint(int row, int col)
{
	printf("SCORE = %d", score);
	SetCurPos(0, 2);
	int i = 0;
	for (i = 0; i < row; i++)
	{
		for (int k = 0; k < 4; k++)
		{
			printf(" ");
			for (int j = 0; j < 4; j++)
			{
				printf("-");
			}
		}
		printf("\n");
		for (int k = 0; k < 4; k++)
		{
			printf("|");
			if (arr[i][k])
			{
				printf("%4d", arr[i][k]);
			}
			else
			{
				for (int j = 0; j < 4; j++)
				{
					printf(" ");
				}
			}

		}
		printf("|\n");
		if (i == row - 1)
		{
			for (int k = 0; k < 4; k++)
			{
				printf(" ");
				for (int j = 0; j < 4; j++)
				{
					printf("-");
				}
			}
			printf("\n");
		}
	}
	SetCurPos(0, 12);
	printf("�������ұ�ʾ����\n");
}
//���������
void RandNum(int row, int col) //���������
{
	int x = 0;
	int y = 0;
	do
	{
		x = rand() % 4;
		y = rand() % 4;
	} while (arr[x][y]);
	arr[x][y] = (rand() % 10 ? 2 : 4);
}
//�ж��Ƿ��ʤ
int IfWin(int row, int col)
{
	for (int i = 0; i < row; i++)
	{
		for (int j = 0; j < col; j++)
		{
			if (arr[i][j] == 2048)
			{
				return 1; //��ʾӮ��
			}
			if (arr[i][j] == 0)
			{
				return 0; //��ʾ������Ϸ
			}
		}
	}
	return -1;//����
}
//������
void MoveLeft()
{
	for (int i = 0; i < ROW; i++)
	{
		int j = 1; 
		int k = 0;
		for (; j < 4; j++)
		{
			if (arr[i][j] > 0)
			{
				if (arr[i][k] == 0)
				{
					arr[i][k] = arr[i][j];
					arr[i][j] = 0;
				}
				else if(arr[i][k] == arr[i][j])
				{
					arr[i][k] = 2 * arr[i][k];
					score += arr[i][k];
					arr[i][j] = 0;
					k++;
				}
				else
				{
					arr[i][++k] = arr[i][j];
					if (j != k)
					{
						arr[i][j] = 0;
					}
				}
			}
		}
	}
}
//������
void MoveRight()
{
	for (int i = 0; i < ROW; i++)
	{
		int j = 2;
		int k = 3;
		for (; j >= 0; j--)
		{
			if (arr[i][j] > 0)
			{
				if (arr[i][k] == 0)
				{
					arr[i][k] = arr[i][j];
					arr[i][j] = 0;
				}
				else if (arr[i][k] == arr[i][j])
				{
					arr[i][k] = 2 * arr[i][k];
					score += arr[i][k];
					arr[i][j] = 0;
					k--;
				}
				else
				{
					arr[i][--k] = arr[i][j];
					if (j != k)
					{
						arr[i][j] = 0;
					}
				}
			}
		}
	}
}
//������
void MoveUp()
{
	for (int j = 0; j < COL; j++)
	{
		int i = 1;
		int k = 0;
		for (; i < 4; i++)
		{
			if (arr[i][j] > 0)
			{
				if (arr[k][j] == arr[i][j])
				{
					score += arr[k++][j] *= 2;
					arr[i][j] = 0;
				}
				else if (arr[k][j] == 0)
				{
					arr[k][j] = arr[i][j];
					arr[i][j] = 0;
				}
				else
				{
					arr[++k][j] = arr[i][j];
					if (i != k)
					{
						arr[i][j] = 0;
					}
				}
			}
		}
	}
}
//������
void MoveDown()
{
	for (int j = 0; j < COL; j++)
	{
		int i = 2;
		int k = 3;
		for (; i >= 0; i--)
		{
			if (arr[i][j] > 0)
			{
				if (arr[k][j] == 0)
				{
					arr[k][j] = arr[i][j];
					arr[i][j] = 0;
				}
				else if (arr[k][j] == arr[i][j])
				{
					arr[k][j] = 2 * arr[k][j];
					score += arr[k][j];
					arr[i][j] = 0;
					k--;
				}
				else
				{
					arr[--k][j] = arr[i][j];
					if (i != k)
					{
						arr[i][j] = 0;
					}
				}
			}
		}
	}
}
//��Ϸ����
void GameContinue()
{
	int ch1 = 0;
	int ch2 = 0;
	while (1)
	{
		if (ch1 = _getch())
		{
			ch2 = _getch();
			if (ch2 == 72)
			{
				MoveUp();
				break;
			}
			if (ch2 == 80)
			{
				MoveDown();
				break;
			}
			if (ch2 == 75)
			{
				MoveLeft();
				break;
			}
			if (ch2 == 77)
			{
				MoveRight();
				break;
			}
		}
	}
}

void game()
{
	srand((unsigned int)time(NULL));
	ViewInit(20, 10);
	RandNum(ROW, COL);
	RandNum(ROW, COL);
	GamePrint(ROW, COL);
	while (1)
	{
		if (IfWin(ROW, COL) == 1)
		{
			printf("YOU WIN!\n");
			return;
		}
		else if (IfWin(ROW, COL) == -1)
		{
			printf("GAME OVER\n");
			return;
		}
		else
		{
			GameContinue();
			system("cls");
			RandNum(ROW, COL);
			GamePrint(ROW, COL);
		}
	}
}

int main()
{
	game();
	system("pause");
	return 0;
}

