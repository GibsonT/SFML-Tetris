#include <SFML\Audio.hpp>
#include <vector>
#include <list>
#include <iostream>
#include <time.h>

#include "Brick.h"
#include "TetrisFunctions.h"

using namespace std;

void CheckInput(Event* gameEvent, Brick* activeBrick, bool gameBoard[GB_HEIGHT][GB_WIDTH], unsigned int* score)
{
	Keyboard::Key keyPressed = (Keyboard::Key)gameEvent->Key.Code;

	if (keyPressed == Keyboard::Left)
	{	
		for (int i = 0; i < SPRITES_IN_BRICK; i++)
		{
			if (activeBrick->GetSpriteXPosition(i) <= BRICK_LEFT_BOUNDRY || 
				IsSideCollision(*activeBrick, gameBoard, true))
			{
				return;
			}
		}

		for (int i = 0; i < SPRITES_IN_BRICK; i++)
		{
			activeBrick->SetSpriteXPosition(i, -1);
		}
	}
	else if (keyPressed == Keyboard::Right)
	{
		for (int i = 0; i < SPRITES_IN_BRICK; i++)
		{
			if (activeBrick->GetSpriteXPosition(i) >= BRICK_RIGHT_BOUNDRY ||
				IsSideCollision(*activeBrick, gameBoard, false, true))
			{
				return;
			}
		}
		
		for (int i = 0; i < SPRITES_IN_BRICK; i++)
		{
			activeBrick->SetSpriteXPosition(i, 1);
		}
	}
	else if (keyPressed == Keyboard::Down)
	{
		for (int i = 0; i < SPRITES_IN_BRICK; i++)
		{
			if (activeBrick->GetSpriteYPosition(i) == BRICK_LOWER_BOUNDRY ||
				IsBottomCollision(*activeBrick, gameBoard))
			{
				activeBrick->LockBrick();
				return;
			}
		}

		for (int i = 0; i < SPRITES_IN_BRICK; i++)
		{
			activeBrick->SetSpriteYPosition(i, 1);
		}

		*score += 1;
	}
	else if (keyPressed == Keyboard::Z)
	{	
		SoundBuffer rotateSoundBuffer;
		rotateSoundBuffer.LoadFromFile("Sounds/rotate.wav");
		Sound rotateSound;
		rotateSound.SetBuffer(rotateSoundBuffer);
		rotateSound.SetVolume(50.0f);

		if (!IsRotationCollision(*activeBrick, gameBoard))
		{
			rotateSound.Play();
			activeBrick->RotateBrick(Brick::Clockwise);

			//Keeps the function active long enough to hear the rotate sound play
			//Probably the hackiest thing ever, but it works for the most part :/
			while (rotateSound.GetStatus() == SoundSource::Playing)
			{
				continue;
			}
		}
	}
	else if (keyPressed == Keyboard::P)
	{
		for (int i = 0; i < GB_HEIGHT; i++)
		{

			cout << "Row " << i << endl;
			
			for (int j = 0; j < GB_WIDTH; j++)
			{
				cout << "\tColumn " << j << " Value: " << gameBoard[i][j] << endl;
			}
		}
	}
}

bool IsBottomCollision(Brick activeBrick, bool gameBoard[GB_HEIGHT][GB_WIDTH])
{
	for (int i = 0; i < SPRITES_IN_BRICK; i++)
	{
		int yPos = activeBrick.GetSpriteYPosition(i);
		int xPos = activeBrick.GetSpriteXPosition(i);
		
		if (yPos == BRICK_LOWER_BOUNDRY || gameBoard[yPos + 1][xPos])
		{
			return true;
		}
	}

	return false;
}

bool IsSideCollision(Brick activeBrick, bool gameBoard[GB_HEIGHT][GB_WIDTH], 
						bool checkLeftSide, bool checkRightSide)
{
	if (checkLeftSide)
	{
		for (int i = 0; i < SPRITES_IN_BRICK; i++)
		{
			int xPos = activeBrick.GetSpriteXPosition(i);
			int yPos = activeBrick.GetSpriteYPosition(i);

			if (xPos == BRICK_LEFT_BOUNDRY || gameBoard[yPos][xPos - 1])
			{
				return true;
			}
		}
	}
	else if (checkRightSide)
	{
		for (int i = 0; i < SPRITES_IN_BRICK; i++)
		{
			int xPos = activeBrick.GetSpriteXPosition(i);
			int yPos = activeBrick.GetSpriteYPosition(i);
			
			if (xPos == BRICK_RIGHT_BOUNDRY || gameBoard[yPos][xPos + 1])
			{
				return true;
			}
		}
	}

	return false;
}

bool IsRotationCollision(Brick activeBrick, bool gameBoard[GB_HEIGHT][GB_WIDTH])
{
	if (activeBrick.GetDirectionFacing() == Brick::NullDirection)
	{
		return false;
	}

	Brick::DirectionType directionFacing = activeBrick.GetDirectionFacing();
	Brick::BrickType brickType = activeBrick.GetBrickType();
	int pivotXPos = activeBrick.GetSpriteXPosition(3);
	int pivotYPos = activeBrick.GetSpriteYPosition(3);
	
	switch (directionFacing)
	{
		case Brick::North:
		{
			switch (brickType)
			{
				case Brick::Line:
				{
					if (pivotXPos + 3 > BRICK_RIGHT_BOUNDRY)
					{
						return true;
					}
					else
					{
						for (int i = 1; i < SPRITES_IN_BRICK; i++)
						{
							if (gameBoard[pivotYPos][pivotXPos + i])
							{
								return true;
							}
						}

						return false;
						break;
					}
				}

				case Brick::Z:
				{
					if (pivotXPos + 2 > BRICK_RIGHT_BOUNDRY ||
						pivotYPos + 1 > BRICK_LOWER_BOUNDRY)
					{
						return true;
					}
					else if (gameBoard[pivotYPos][pivotXPos + 1] ||
							 gameBoard[pivotYPos + 1][pivotXPos + 1] ||
							 gameBoard[pivotYPos + 1][pivotXPos + 2])
					{
						return true;
					}

					return false;
					break;
				}

				case Brick::ReverseZ:
				{
					if (pivotXPos + 2 > BRICK_RIGHT_BOUNDRY )
					{
						return true;
					}
					else if (gameBoard[pivotYPos][pivotXPos + 1] ||
							 gameBoard[pivotYPos - 1][pivotXPos + 1] ||
							 gameBoard[pivotYPos - 1][pivotXPos + 2])
					{
						return true;
					}

					return false;
					break;
				}

				case Brick::L:
				{
					if (pivotXPos + 2 > BRICK_RIGHT_BOUNDRY)
					{
						return true;
					}
					else if (gameBoard[pivotYPos - 1][pivotXPos] ||
							 gameBoard[pivotYPos - 1][pivotXPos + 1] ||
							 gameBoard[pivotYPos - 1][pivotXPos + 2])
					{
						return true;
					}

					return false;
					break;
				}

				case Brick::ReverseL:
				{
					if (pivotXPos + 2 > BRICK_RIGHT_BOUNDRY ||
						pivotYPos + 1 > BRICK_LOWER_BOUNDRY )
					{
						return true;
					}
					else if (gameBoard[pivotYPos + 1][pivotXPos] ||
							 gameBoard[pivotYPos + 1][pivotXPos + 1] ||
							 gameBoard[pivotYPos + 1][pivotXPos + 2])
					{
						return true;
					}

					return false;
					break;
				}

				case Brick::T:
				{
					if (pivotXPos + 1 > BRICK_RIGHT_BOUNDRY ||
						pivotYPos + 1 > BRICK_LOWER_BOUNDRY)
					{
						return true;
					}
					else if (gameBoard[pivotYPos - 1][pivotXPos + 1] ||
							 gameBoard[pivotYPos][pivotXPos + 1] ||
							 gameBoard[pivotYPos + 1][pivotXPos + 1])
					{
						return true;
					}

					return false;
					break;
				}
			}	//END BRICKTYPE SWITCH

			break;
		}	//END DIRECTIONTYPE CASE

		case Brick::East:
		{
			switch (brickType)
			{
				case Brick::Line:
				{
					if (pivotYPos + 3 > BRICK_LOWER_BOUNDRY)
					{
						return true;
					}
					else
					{
						for (int i = 1; i < SPRITES_IN_BRICK; i++)
						{
							if (gameBoard[pivotYPos + i][pivotXPos])
							{
								return true;
							}
						}

						return false;
						break;
					}
				}

				case Brick::Z:
				{
					if (pivotYPos + 2 > BRICK_LOWER_BOUNDRY ||
						pivotXPos - 1 < BRICK_LEFT_BOUNDRY)
					{
						return true;
					}
					else if (gameBoard[pivotYPos + 1][pivotXPos] ||
							 gameBoard[pivotYPos + 1][pivotXPos - 1] ||
							 gameBoard[pivotYPos + 2][pivotXPos - 1])
					{
						return true;
					}

					return false;
					break;
				}

				case Brick::ReverseZ:
				{
					if (pivotYPos + 2 > BRICK_LOWER_BOUNDRY ||
						pivotXPos + 1 > BRICK_RIGHT_BOUNDRY)
					{
						return true;
					}
					else if (gameBoard[pivotYPos + 1][pivotXPos] ||
							 gameBoard[pivotYPos + 1][pivotXPos + 1] ||
							 gameBoard[pivotYPos + 2][pivotXPos + 1])
					{
						return true;
					}

					return false;
					break;
				}

				case Brick::L:
				{
					if (pivotYPos + 2 > BRICK_LOWER_BOUNDRY ||
						pivotXPos + 1 > BRICK_RIGHT_BOUNDRY)
					{
						return true;
					}
					else if (gameBoard[pivotYPos][pivotXPos + 1] ||
							 gameBoard[pivotYPos + 1][pivotXPos + 1] ||
							 gameBoard[pivotYPos + 2][pivotXPos + 1])
					{
						return true;
					}

					return false;
					break;
				}

				case Brick::ReverseL:
				{
					if (pivotYPos + 2 > BRICK_LOWER_BOUNDRY ||
						pivotXPos - 1 < BRICK_LEFT_BOUNDRY)
					{
						return true;
					}
					else if (gameBoard[pivotYPos][pivotXPos - 1] ||
							 gameBoard[pivotYPos + 1][pivotXPos - 1] ||
							 gameBoard[pivotYPos + 2][pivotXPos - 1])
					{
						return true;
					}

					return false;
					break;
				}

				case Brick::T:
				{
					if (pivotXPos + 1 > BRICK_RIGHT_BOUNDRY ||
						pivotXPos - 1 < BRICK_LEFT_BOUNDRY ||
						pivotYPos + 1 > BRICK_LOWER_BOUNDRY)
					{
						return true;
					}
					else if (gameBoard[pivotYPos - 1][pivotXPos + 1] ||
							 gameBoard[pivotYPos][pivotXPos + 1] ||
							 gameBoard[pivotYPos + 1][pivotXPos + 1])
					{
						return true;
					}

					return false;
					break;
				}
			}	//END BRICKTYPE SWITCH

			break;
		}	//END DIRECTIONTYPE CASE

		case Brick::South:
		{
			switch (brickType)
			{
				case Brick::Line:
				{
					if (pivotXPos - 3 < BRICK_LEFT_BOUNDRY)
					{
						return true;
					}
					else
					{
						for (int i = 1; i < SPRITES_IN_BRICK; i++)
						{
							if (gameBoard[pivotYPos][pivotXPos - i])
							{
								return true;
							}
						}

						return false;
						break;
					}
				}

				case Brick::Z:
				{
					if (pivotXPos - 2 < BRICK_LEFT_BOUNDRY)
					{
						return true;
					}
					else if (gameBoard[pivotYPos][pivotXPos - 1] ||
							 gameBoard[pivotYPos - 1][pivotXPos - 1] ||
							 gameBoard[pivotYPos - 1][pivotXPos - 2])
					{
						return true;
					}

					return false;
					break;
				}

				case Brick::ReverseZ:
				{
					if (pivotXPos - 2 < BRICK_LEFT_BOUNDRY ||
						pivotYPos + 1 > BRICK_LOWER_BOUNDRY)
					{
						return true;
					}
					else if (gameBoard[pivotYPos][pivotXPos - 1] ||
							 gameBoard[pivotYPos + 1][pivotXPos - 1] ||
							 gameBoard[pivotYPos + 1][pivotXPos - 2])
					{
						return true;
					}

					return false;
					break;
				}

				case Brick::L:
				{
					if (pivotXPos - 2 < BRICK_LEFT_BOUNDRY ||
						pivotYPos + 1 > BRICK_LOWER_BOUNDRY)
					{
						return true;
					}
					else if (gameBoard[pivotYPos + 1][pivotXPos] ||
							 gameBoard[pivotYPos + 1][pivotXPos - 1] ||
							 gameBoard[pivotYPos + 1][pivotXPos - 2])
					{
						return true;
					}

					return false;
					break;
				}

				case Brick::ReverseL:
				{
					if (pivotXPos - 2 < BRICK_LEFT_BOUNDRY)
					{
						return true;
					}
					else if (gameBoard[pivotYPos - 1][pivotXPos] ||
							 gameBoard[pivotYPos - 1][pivotXPos - 1] ||
							 gameBoard[pivotYPos - 1][pivotXPos - 2])
					{
						return true;
					}

					return false;
					break;
				}

				case Brick::T:
				{
					if (pivotXPos - 1 < BRICK_LEFT_BOUNDRY ||
						pivotYPos + 1 > BRICK_LOWER_BOUNDRY)
					{
						return true;
					}
					else if (gameBoard[pivotYPos - 1][pivotXPos - 1] ||
							 gameBoard[pivotYPos][pivotXPos - 1] ||
							 gameBoard[pivotYPos + 1][pivotXPos - 1])
					{
						return true;
					}

					return false;
					break;
				}
			}	//END BRICKTYPE SWITCH

			break;
		}	//END DIRECTIONTYPE CASE

		case Brick::West:
		{
			switch (brickType)
			{
				case Brick::Line:
				{
					for (int i = 1; i < SPRITES_IN_BRICK; i++)
					{
						if (gameBoard[pivotYPos - i][pivotXPos])
						{
							return true;
						}
					}

					return false;
					break;
				}

				case Brick::Z:
				{
					if (pivotXPos + 1 > BRICK_RIGHT_BOUNDRY)
					{
						return true;
					}
					else if (gameBoard[pivotYPos - 1][pivotXPos] ||
							 gameBoard[pivotYPos - 1][pivotXPos + 1] ||
							 gameBoard[pivotYPos - 2][pivotXPos + 1])
					{
						return true;
					}

					return false;
					break;
				}

				case Brick::ReverseZ:
				{
					if (pivotXPos - 1 < BRICK_LEFT_BOUNDRY)
					{
						return true;
					}
					else if (gameBoard[pivotYPos - 1][pivotXPos] ||
							 gameBoard[pivotYPos - 1][pivotXPos - 1] ||
							 gameBoard[pivotYPos - 2][pivotXPos - 1])
					{
						return true;
					}

					return false;
					break;
				}

				case Brick::L:
				{
					if (pivotXPos - 1 < BRICK_LEFT_BOUNDRY)
					{
						return true;
					}
					else if (gameBoard[pivotYPos][pivotXPos - 1] ||
							 gameBoard[pivotYPos - 1][pivotXPos - 1] ||
							 gameBoard[pivotYPos - 2][pivotXPos - 1])
					{
						return true;
					}

					return false;
					break;
				}

				case Brick::ReverseL:
				{
					if (pivotXPos + 1 > BRICK_RIGHT_BOUNDRY)
					{
						return true;
					}
					else if (gameBoard[pivotYPos][pivotXPos + 1] ||
							 gameBoard[pivotYPos - 1][pivotXPos + 1] ||
							 gameBoard[pivotYPos - 2][pivotXPos + 1])
					{
						return true;
					}

					return false;
					break;
				}

				case Brick::T:
				{
					if (pivotXPos - 1 < BRICK_LEFT_BOUNDRY ||
						pivotXPos + 1 > BRICK_RIGHT_BOUNDRY)
					{
						return true;
					}
					else if (gameBoard[pivotYPos - 1][pivotXPos - 1] ||
							 gameBoard[pivotYPos - 1][pivotXPos] ||
							 gameBoard[pivotYPos - 1][pivotXPos + 1])
					{
						return true;
					}

					return false;
					break;
				}
			}	//END BRICKTYPE SWITCH

			break;
		}	//END DIRECTIONTYPE CASE
	}
}

void ClearRow(vector<Brick>* brickList, int yPosToClear, bool gameBoard[GB_HEIGHT][GB_WIDTH])
{
	for (int i = 0; i < GB_WIDTH; i++)
	{
		gameBoard[yPosToClear][i] = false;
	}

	for (int i = 0; i < brickList->size(); i++)
	{
		Brick* currentBrick = &brickList->at(i);

		for (int j = 0; j < SPRITES_IN_BRICK; j++)
		{
			if (currentBrick->GetSpriteYPosition(j) == yPosToClear)
			{
				currentBrick->MarkSpriteForDeletion(j);
				currentBrick->RemoveSprite(j);
			}

			else if (currentBrick->GetSpriteYPosition(j) < yPosToClear && !currentBrick->GetSpriteDeletionStatus(j))
			{
				gameBoard[currentBrick->GetSpriteYPosition(j)][currentBrick->GetSpriteXPosition(j)] = false;
				currentBrick->SetSpriteYPosition(j, 1);
			}
		}
	}
	
	//CleanupBricks(brickList);

	for (int i = 0; i < brickList->size(); i++)
	{
		Brick currentBrick = brickList->at(i);
		
		for (int j = 0; j < SPRITES_IN_BRICK; j++)
		{
			if (!currentBrick.GetSpriteDeletionStatus(j))
			{
				gameBoard[currentBrick.GetSpriteYPosition(j)][currentBrick.GetSpriteXPosition(j)] = true;
			}
		}
	}
}

void CleanupBricks(vector<Brick>* brickList)
{
	list<int> bricksToDelete;

	for (int i = 0; i < brickList->size(); i++)
	{
		Brick currentBrick = brickList->at(i);
		
		for (int j = 0; j < SPRITES_IN_BRICK; j++)
		{
			if (currentBrick.GetSpriteDeletionStatus(j))
			{
				if (j == SPRITES_IN_BRICK - 1)
				{
					bricksToDelete.push_front(i);
				}
				else
				{
					continue;
				}
			}
			else
			{
				break;
			}
		}
	}

	if (bricksToDelete.size() != 0)
	{
		for (int i = 0; i < bricksToDelete.size(); i++)
		{
			vector<Brick>::iterator iter = brickList->begin();
			list<int>::iterator bricksToDeleteIter = bricksToDelete.begin();

			for (int j = i; j > 0; j--)
			{
				bricksToDeleteIter++;
			}
			
			while (*iter != brickList->at(*bricksToDeleteIter))
			{
				iter++;
			}

			brickList->erase(iter);
		}
	}
}

void RefillBrickQueue(vector<Brick>* brickQueue)
{
	brickQueue->clear();
	vector<Brick> tempQueue;
	Texture brickTex;
	brickTex.LoadFromFile("Images/brick.png");
	srand(time(nullptr));

	for (int i = 0; i < BRICKS_IN_QUEUE; i++)
	{
		for (int j = 0; j < NUMBER_OF_BRICK_TYPES; j++)
		{
			for (int k = 0; k < 2; k++)
			{
				Brick newBrick((Brick::BrickType)j);
				tempQueue.push_back(newBrick);
				i++;
			}
		}
	}

	while (tempQueue.size() > 0)
	{
		int rng = (double)rand() / (RAND_MAX + 1) * (tempQueue.size() - 1);
		vector<Brick>::iterator iter = tempQueue.begin();
		brickQueue->push_back(tempQueue.at(rng));
		
		while (iter != tempQueue.end())
		{
			if (*iter != tempQueue.at(rng))
			{
				iter++;
			}
			else
			{
				break;
			}
		}

		tempQueue.erase(iter);
	}

	for (int i = 0; i < brickQueue->size(); i++)
	{
		brickQueue->at(i).ResetTexture();
	}
}

Brick PullBrickFromQueue(vector<Brick>* brickQueue)
{
	Brick* newBrick = &brickQueue->at(brickQueue->size() - 1);
	brickQueue->pop_back();

	if (brickQueue->size() == 0)
	{
		RefillBrickQueue(brickQueue);
	}

	newBrick->ResetTexture();
	return *newBrick;
}