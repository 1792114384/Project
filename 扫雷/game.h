#ifndef __GAME_H__
#define __GAME_H__

#define ROW 9
#define COL 9

#define ROWS ROW+2
#define COLS COL+2
#define EASY_COUNT 10 //��������

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

void InitBoard(char board[ROWS][COLS], int row, int col, char c);//��ʼ������
void SetBoard(char board[ROWS][COLS], int row, int col);//����
void PrintBoard(char board[ROWS][COLS], int row, int col);//��ӡ����
int AroundCount(char board[ROWS][COLS], int x, int y);//������Χ����
int IfWin(char board[ROWS][COLS], int row, int col);//�ж���Ӯ
void Minesweeper(char MineBoard[ROWS][COLS], char PlayBoard[ROWS][COLS], int row, int col);//ɨ�׹���
void FirstStep(char MineBoard[ROWS][COLS], char PlayBoard[ROWS][COLS], int row, int col, int x, int y);//��һ��
void OpenBoard(char MineBoard[ROWS][COLS], char PlayBoard[ROWS][COLS], int x, int y);//��һ��չ��

#endif