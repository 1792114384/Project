#ifndef __VIEW_H__
#define __VIEW_H__

#include "Model.h"

void ViewInit(int width, int height);
void SetCurPos(int x, int y);
void DisplayWall(int width, int height);
void DisplaySnake(Snake *pSnake);
void DisplayFood(int x, int y);
void DisplayScore(int score);
void DisplaySnakeBlock(int x, int y);
void CleanSnakeBlock(int x, int y);

#endif  //__VIEW_H__