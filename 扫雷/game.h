#ifndef __GAME_H__
#define __GAME_H__

#define ROW 9
#define COL 9

#define ROWS ROW+2
#define COLS COL+2
#define EASY_COUNT 10 //设置雷数

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

void InitBoard(char board[ROWS][COLS], int row, int col, char c);//初始化棋盘
void SetBoard(char board[ROWS][COLS], int row, int col);//布雷
void PrintBoard(char board[ROWS][COLS], int row, int col);//打印棋盘
int AroundCount(char board[ROWS][COLS], int x, int y);//计算周围雷数
int IfWin(char board[ROWS][COLS], int row, int col);//判断输赢
void Minesweeper(char MineBoard[ROWS][COLS], char PlayBoard[ROWS][COLS], int row, int col);//扫雷过程
void FirstStep(char MineBoard[ROWS][COLS], char PlayBoard[ROWS][COLS], int row, int col, int x, int y);//第一步
void OpenBoard(char MineBoard[ROWS][COLS], char PlayBoard[ROWS][COLS], int x, int y);//第一步展开

#endif