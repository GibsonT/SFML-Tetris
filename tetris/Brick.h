#ifndef BRICK_H
#define BRICK_H

#include <SFML\Graphics.hpp>
#include <SFML\Audio.hpp>
#include "Defines.h"

using namespace sf;
using namespace std;

class Brick
{
public:
	enum BrickType
	{
		Box,
		Line,
		Z,
		ReverseZ,
		L,
		ReverseL,
		T
	};

	enum RotationType
	{
		Clockwise,
		CounterClockwise
	};

	enum DirectionType
	{
		North,
		South,
		East,
		West,
		NullDirection
	};

private:
	Sprite brickSprite[SPRITES_IN_BRICK];
	BrickType brickType;
	DirectionType directionFacing;
	unsigned int SpriteGBXPosition[SPRITES_IN_BRICK];
	unsigned int SpriteGBYPosition[SPRITES_IN_BRICK];
	bool isLocked;
	bool isSpriteMarkedForDeletion[SPRITES_IN_BRICK];
	int pivotSprite;	//The index for the sprite for the other sprites to pivot around.
	Texture brickTexture;

public:
	void LockBrick()
		{ isLocked = true; }
	
	bool IsLocked()
		{ return isLocked; }

	Sprite* GetSpriteArray()
		{ return brickSprite; }

	int GetSpriteXPosition(int spriteArrayIndex)
		{ return this->SpriteGBXPosition[spriteArrayIndex]; }

	int GetSpriteYPosition(int spriteArrayIndex)
		{ return this->SpriteGBYPosition[spriteArrayIndex]; }

	int GetSpriteHeight(int spriteArrayIndex)
		{ return this->GetSpriteArray()[spriteArrayIndex].GetTexture()->GetHeight(); }

	int GetSpriteWidth(int spriteArrayIndex)
		{ return this->GetSpriteArray()[spriteArrayIndex].GetTexture()->GetWidth(); }

	Texture GetBrickTexture()
		{ return brickTexture; }

	DirectionType GetDirectionFacing()
		{ return this->directionFacing; }

	BrickType GetBrickType()
		{ return this->brickType; }

	bool GetSpriteDeletionStatus(int spriteArrayIndex)
		{ return isSpriteMarkedForDeletion[spriteArrayIndex]; }

	void SetSpriteXPosition(int spriteArrayIndex, int positionOffset);

	void SetSpriteYPosition(int spriteArrayIndex, int positionOffset);

	void SetSpriteGBPositions(int spriteArrayIndex, int xPositionOffset, int yPositionOffset);

	void SetPreviewPosition();

	void SetActivePosition();
	
	void SetInitialPositions(BrickType brickType);

	//Sets the current sprite to a new blank sprite.
	//Temp fix.
	void RemoveSprite (int spriteArrayIndex);

	void RotateBrick(RotationType rotationType);

	void ResetTexture();

	bool operator!=(Brick brick);

	void MarkSpriteForDeletion(int spriteArrayIndex)
		{ isSpriteMarkedForDeletion[spriteArrayIndex] = true; }
	
	Brick(BrickType brickType);
};

#endif