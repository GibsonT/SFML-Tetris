#ifndef TETFUNC_H
#define TETFUNC_H

#include "Brick.h"
#include "Defines.h"
#include <vector>

using namespace std;


void CheckInput(Event* gameEvent, Brick* activeBrick, bool gameBoard[GB_HEIGHT][GB_WIDTH], unsigned int* score);

bool IsBottomCollision(Brick activeBrick, bool gameBoard[GB_HEIGHT][GB_WIDTH]);

bool IsSideCollision(Brick activeBrick, bool gameBoard[GB_HEIGHT][GB_WIDTH], 
						bool checkLeftSide = false, bool checkRightSide = false);

bool IsRotationCollision(Brick activeBrick, bool gameBoard[GB_HEIGHT][GB_WIDTH]);

void ClearRow(vector<Brick>* brickList, int yPosToClear, bool gameBoard[GB_HEIGHT][GB_WIDTH]);

void ResetGameBoardRow(bool gameBoard[GB_HEIGHT][GB_WIDTH], int yPosOfRowToReset);

void CleanupBricks(vector<Brick>* brickList);

void RefillBrickQueue(vector<Brick>* brickQueue);

Brick PullBrickFromQueue(vector<Brick>* brickQueue);

#endif