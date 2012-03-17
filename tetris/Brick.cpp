#include "Brick.h"
#include "TetrisFunctions.h"

void Brick::SetSpriteXPosition(int spriteArrayIndex, int positionOffset)
{ 
	this->SpriteGBXPosition[spriteArrayIndex] += positionOffset;
		
	float yPos = (SpriteGBYPosition[spriteArrayIndex] * 20);
	float xPos = (GB_X_OFFSET + SpriteGBXPosition[spriteArrayIndex]) * 20;
	this->GetSpriteArray()[spriteArrayIndex].SetPosition(xPos, yPos);
}

void Brick::SetSpriteYPosition(int spriteArrayIndex, int positionOffset)
{
	this->SpriteGBYPosition[spriteArrayIndex] += positionOffset;
		
	float xPos = (GB_X_OFFSET + SpriteGBXPosition[spriteArrayIndex]) * 20;
	float yPos = (SpriteGBYPosition[spriteArrayIndex] * 20);
	this->GetSpriteArray()[spriteArrayIndex].SetPosition(xPos, yPos);
}

void Brick::SetSpriteGBPositions(int spriteArrayIndex, int xPositionOffset, int yPositionOffset)
{
	SetSpriteXPosition(spriteArrayIndex, xPositionOffset);
	SetSpriteYPosition(spriteArrayIndex, yPositionOffset);
}

void Brick::SetPreviewPosition()
{
	for (int i = 0; i < SPRITES_IN_BRICK; i++)
	{
		this->SetSpriteXPosition(i, 9);
		this->SetSpriteYPosition(i, 8);
	}
}

void Brick::SetActivePosition()
{
	for (int i = 0; i < SPRITES_IN_BRICK; i++)
	{
		this->SetSpriteXPosition(i, -9);
		this->SetSpriteYPosition(i, -8);
	}
}

void Brick::SetInitialPositions(BrickType brickType)
{
	switch (brickType)
	{
	case Brick::Box:
		SetSpriteGBPositions(0, 4, 1);
		SetSpriteGBPositions(1, 4, 2);
		SetSpriteGBPositions(2, 5, 1);
		SetSpriteGBPositions(3, 5, 2);
		break;
	case Brick::Line:
		SetSpriteGBPositions(0, 3, 2);
		SetSpriteGBPositions(1, 4, 2);
		SetSpriteGBPositions(2, 5, 2);
		SetSpriteGBPositions(3, 6, 2);
		break;
	case Brick::Z:
		SetSpriteGBPositions(0, 3, 1);
		SetSpriteGBPositions(1, 4, 1);
		SetSpriteGBPositions(2, 4, 2);
		SetSpriteGBPositions(3, 5, 2);
		break;
	case Brick::ReverseZ:
		SetSpriteGBPositions(0, 3, 2);
		SetSpriteGBPositions(1, 4, 2);
		SetSpriteGBPositions(2, 4, 1);
		SetSpriteGBPositions(3, 5, 1);
		break;
	case Brick::L:
		SetSpriteGBPositions(0, 4, 0);
		SetSpriteGBPositions(1, 4, 1);
		SetSpriteGBPositions(2, 4, 2);
		SetSpriteGBPositions(3, 5, 2);
		break;
	case Brick::ReverseL:
		SetSpriteGBPositions(0, 5, 0);
		SetSpriteGBPositions(1, 5, 1);
		SetSpriteGBPositions(2, 5, 2);
		SetSpriteGBPositions(3, 4, 2);
		break;
	case Brick::T:
		SetSpriteGBPositions(0, 3, 1);
		SetSpriteGBPositions(1, 4, 1);
		SetSpriteGBPositions(2, 5, 1);
		SetSpriteGBPositions(3, 4, 2);
	default:
		break;
	}
}

//Sets the current sprite to a new blank sprite.
//Temp fix.
void Brick::RemoveSprite (int spriteArrayIndex)
{
	Sprite* blankSprite = new Sprite();
	this->GetSpriteArray()[spriteArrayIndex] = *blankSprite;
}

//Here be dragons, enter at your own risk!
void Brick::RotateBrick(RotationType rotationType)
{
	//Rotating a box doesn't matter, so exit the function.
	if (brickType == Brick::Box)
	{
		return;
	}
	

	//I was once a great programmer like you, but then a fucking dragon bit my face off.
	switch (directionFacing)
	{
		if (rotationType == Brick::Clockwise)
		{
			case Brick::West:
			{
				this->directionFacing = Brick::North;
					
				switch (brickType)
				{
				case Brick::Line:
					SetSpriteGBPositions(0, 3, -3);
					SetSpriteGBPositions(1, 2, -2);
					SetSpriteGBPositions(2, 1, -1);
					break;
				case Brick::Z:
					SetSpriteGBPositions(0, 3, -1);
					SetSpriteGBPositions(1, 2, 0);
					SetSpriteGBPositions(2, 1, -1);
					break;
				case Brick::ReverseZ:
					SetSpriteGBPositions(0, 1, -3);
					SetSpriteGBPositions(1, 0, -2);
					SetSpriteGBPositions(2, 1, -1);
					break;
				case Brick::L:
					SetSpriteGBPositions(0, 1, -3);
					SetSpriteGBPositions(1, 0, -2);
					SetSpriteGBPositions(2, -1, -1);
					break;
				case Brick::ReverseL:
					SetSpriteGBPositions(0, 3, -1);
					SetSpriteGBPositions(1, 2, 0);
					SetSpriteGBPositions(2, 1, 1);
					break;
				case Brick::T:
					SetSpriteGBPositions(0, 0, -2);
					SetSpriteGBPositions(1, 1, -1);
					SetSpriteGBPositions(2, 2, 0);
					break;
				default:
					break;
				}

				break;
			}

			case Brick::North:
			{
				this->directionFacing = Brick::East;

				switch (brickType)
				{
				case Brick::Line:
					SetSpriteGBPositions(0, 3, 3);
					SetSpriteGBPositions(1, 2, 2);
					SetSpriteGBPositions(2, 1, 1);
					break;
				case Brick::Z:
					SetSpriteGBPositions(0, 1, 3);
					SetSpriteGBPositions(1, 0, 2);
					SetSpriteGBPositions(2, 1, 1);
					break;
				case Brick::ReverseZ:
					SetSpriteGBPositions(0, 3, 1);
					SetSpriteGBPositions(1, 2, 0);
					SetSpriteGBPositions(2, 1, 1);
					break;
				case Brick::L:
					SetSpriteGBPositions(0, 3, 1);
					SetSpriteGBPositions(1, 2, 0);
					SetSpriteGBPositions(2, 1, -1);
					break;
				case Brick::ReverseL:
					SetSpriteGBPositions(0, 1, 3);
					SetSpriteGBPositions(1, 0, 2);
					SetSpriteGBPositions(2, -1, 1);
					break;
				case Brick::T:
					SetSpriteGBPositions(0, 2, 0);
					SetSpriteGBPositions(1, 1, 1);
					SetSpriteGBPositions(2, 0, 2);
					break;
				default:
					break;
				}

				break;
			}

			case Brick::East:
			{
				this->directionFacing = Brick::South;

				switch (brickType)
				{
				case Brick::Line:
					SetSpriteGBPositions(0, -3, 3);
					SetSpriteGBPositions(1, -2, 2);
					SetSpriteGBPositions(2, -1, 1);
					break;
				case Brick::Z:
					SetSpriteGBPositions(0, -3, 1);
					SetSpriteGBPositions(1, -2, 0);
					SetSpriteGBPositions(2, -1, 1);
					break;
				case Brick::ReverseZ:
					SetSpriteGBPositions(0, -1, 3);
					SetSpriteGBPositions(1, 0, 2);
					SetSpriteGBPositions(2, -1, 1);
					break;
				case Brick::L:
					SetSpriteGBPositions(0, -1, 3);
					SetSpriteGBPositions(1, 0, 2);
					SetSpriteGBPositions(2, 1, 1);
					break;
				case Brick::ReverseL:
					SetSpriteGBPositions(0, -3, 1);
					SetSpriteGBPositions(1, -2, 0);
					SetSpriteGBPositions(2, -1, -1);
					break;
				case Brick::T:
					SetSpriteGBPositions(0, 0, 2);
					SetSpriteGBPositions(1, -1, 1);
					SetSpriteGBPositions(2, -2, 0);
					break;
				default:
					break;
				}

				break;
			}

			case Brick::South:
			{
				this->directionFacing = Brick::West;

				switch (brickType)
				{
				case Brick::Line:
					SetSpriteGBPositions(0, -3, -3);
					SetSpriteGBPositions(1, -2, -2);
					SetSpriteGBPositions(2, -1, -1);
					break;
				case Brick::Z:
					SetSpriteGBPositions(0, -1, -3);
					SetSpriteGBPositions(1, 0, -2);
					SetSpriteGBPositions(2, -1, -1);
					break;
				case Brick::ReverseZ:
					SetSpriteGBPositions(0, -3, -1);
					SetSpriteGBPositions(1, -2, 0);
					SetSpriteGBPositions(2, -1, -1);
					break;
				case Brick::L:
					SetSpriteGBPositions(0, -3, -1);
					SetSpriteGBPositions(1, -2, 0);
					SetSpriteGBPositions(2, -1, 1);
					break;
				case Brick::ReverseL:
					SetSpriteGBPositions(0, -1, -3);
					SetSpriteGBPositions(1, 0, -2);
					SetSpriteGBPositions(2, 1, -1);
					break;
				case Brick::T:
					SetSpriteGBPositions(0, -2, 0);
					SetSpriteGBPositions(1, -1, -1);
					SetSpriteGBPositions(2, 0, -2);
					break;
				default:
					break;
				}

				break;
			}
		}
	}
}

void Brick::ResetTexture()
{
	brickTexture.LoadFromFile("Images/brick.png");
		
	for (int i = 0; i < SPRITES_IN_BRICK; i++)
	{
		this->GetSpriteArray()[i].SetTexture(brickTexture);
	}
}

bool Brick::operator!=(Brick brick)
{
	for (int i = 0; i < SPRITES_IN_BRICK; i++)
	{
		if (this->GetSpriteXPosition(i) == brick.GetSpriteXPosition(i) &&
			this->GetSpriteYPosition(i) == brick.GetSpriteYPosition(i) &&
			this->brickType == brick.brickType &&
			this->directionFacing == brick.directionFacing)
		{
			return false;
		}
	}

	return true;
}

Brick::Brick(BrickType brickType)
{
	isLocked = false;
	this->brickTexture.LoadFromFile("Images/brick.png");
	this->brickType = brickType;

	for (int i = 0; i < SPRITES_IN_BRICK; i++)
	{
		switch (brickType)
		{
		case Brick::Box:
			brickSprite[i].SetColor(Color(255, 162, 21));	//Orange
			this->directionFacing = Brick::NullDirection;
			break;
		case Brick::Line:
			brickSprite[i].SetColor(Color(255, 0, 0));		//Red
			this->directionFacing = Brick::West;
			break;
		case Brick::Z:
			brickSprite[i].SetColor(Color(255, 255, 0));	//Yellow
			this->directionFacing = Brick::West;
			break;
		case Brick::ReverseZ:
			brickSprite[i].SetColor(Color(0, 255, 0));		//Green
			this->directionFacing = Brick::West;
			break;
		case Brick::L:
			brickSprite[i].SetColor(Color(0, 0, 255));		//Blue
			this->directionFacing = Brick::North;
			break;
		case Brick::ReverseL:
			brickSprite[i].SetColor(Color(255, 100, 173));	//Purple
			this->directionFacing = Brick::North;
			break;
		case Brick::T:
			brickSprite[i].SetColor(Color(0, 128, 128));	//Cyan
			this->directionFacing = Brick::North;
			break;
		default:
			break;
		}

		this->brickSprite[i].SetTexture(brickTexture);
		this->SpriteGBXPosition[i] = 0;
		this->SpriteGBYPosition[i] = 0;

		isSpriteMarkedForDeletion[i] = false;
	}

	SetInitialPositions(brickType);
}