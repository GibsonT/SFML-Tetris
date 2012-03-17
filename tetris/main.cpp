#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <SFML/System.hpp>
#include <SFML/Window/Mouse.hpp>
#include <vector>
#include <random>
#include <iostream>
#include <stdlib.h>
#include <time.h>
#include <string>

#include "TetrisFunctions.h"
#include "Brick.h"
#include "Defines.h"

using namespace sf;
using namespace std;

int main()
{
	enum GameState
	{
		MainMenu,
		Playing,
		GameOver
	};
	
	GameState state = GameState::MainMenu;
	VideoMode videoMode(SCREEN_WIDTH, SCREEN_HEIGHT);
	RenderWindow renderWindow(videoMode, "SFML Tetris");
	Event gameEvent;
	Texture backgroundTexture;
	Texture brickPreviewBackground;
	Texture gameOverBackground;
	Texture mainMenuBackground;
	Sprite mainMenuBackgroundSprite;
	Sprite brickPreviewSprite;
	Sprite backgroundSprite;
	Sprite gameOverBackgroundSprite;
	SoundBuffer dropSoundBuffer;
	Sound dropSound;
	Music music;
	Text scoreHeaderText("Score: ");
	Text scoreDisplayText;
	Text levelHeaderText("Level: ");
	Text levelDisplayText;
	Text nextBrickText("Next: ");
	Text gameOverHeaderText("\t  GAME OVER!\nHere are your final stats:");
	Text mainMenuHeaderText("SFML TETRIS!");
	Text mainMenuStartText("Start");
	Text mainMenuQuitText("Quit");
	Text brickCountText[NUMBER_OF_BRICK_TYPES];
	bool gameBoard[GB_HEIGHT][GB_WIDTH];
	unsigned int score = 0;
	unsigned int level = 1;
	unsigned int totalLines = 0;
	unsigned int brickCount[NUMBER_OF_BRICK_TYPES] = {0};
	vector<Brick> brickQueue;
	vector<Brick> brickList;
	Clock brickLockTimer;
	Clock brickFallTimer;
	bool isCheckingLockTime = false;

	backgroundTexture.LoadFromFile("Images/background.png");
	brickPreviewBackground.LoadFromFile("Images/previewBG.png");
	gameOverBackground.LoadFromFile("Images/gameOverBG.png");
	mainMenuBackground.LoadFromFile("Images/mainMenuBG.png");
	
	brickPreviewSprite.SetTexture(brickPreviewBackground);
	brickPreviewSprite.SetPosition(450, 150);
	
	backgroundSprite.SetTexture(backgroundTexture);

	gameOverBackgroundSprite.SetTexture(gameOverBackground);
	gameOverBackgroundSprite.SetPosition(110, 82);
	
	mainMenuBackgroundSprite.SetTexture(mainMenuBackground);

	scoreHeaderText.SetCharacterSize(TEXT_CHAR_SIZE);
	scoreHeaderText.SetPosition(450, 20);
	
	char* scoreDisplayBuffer = new char;
	scoreDisplayText.SetCharacterSize(TEXT_CHAR_SIZE);
	scoreDisplayText.SetPosition(520, 20);
	
	levelHeaderText.SetCharacterSize(TEXT_CHAR_SIZE);
	levelHeaderText.SetPosition(450, 60);
	
	char* levelDisplayBuffer = new char;
	levelDisplayText.SetCharacterSize(TEXT_CHAR_SIZE);
	levelDisplayText.SetPosition(520, 60);
	
	nextBrickText.SetCharacterSize(TEXT_CHAR_SIZE);
	nextBrickText.SetPosition(450, 120);

	gameOverHeaderText.SetCharacterSize(TEXT_CHAR_SIZE);
	gameOverHeaderText.SetPosition(210, 265);

	mainMenuHeaderText.SetCharacterSize(40);
	mainMenuHeaderText.SetPosition(180, 50);

	mainMenuStartText.SetCharacterSize(32);
	mainMenuStartText.SetPosition(285, 210);

	mainMenuQuitText.SetCharacterSize(32);
	mainMenuQuitText.SetPosition(285, 310);

	char* brickCountTextBuffer[NUMBER_OF_BRICK_TYPES];
	for (int i = 0; i < NUMBER_OF_BRICK_TYPES; i++)
	{
		brickCountTextBuffer[i] = new char;
		brickCountText[i].SetPosition(100, 50 * (i + 1));
	}

	dropSoundBuffer.LoadFromFile("Sounds/drop.wav");
	dropSound.SetBuffer(dropSoundBuffer);

	for (int i = 0; i < GB_HEIGHT; i++)
	{
		for (int j = 0; j < GB_WIDTH; j++)
		{
			gameBoard[i][j] = false;
		}
	}

	RefillBrickQueue(&brickQueue);
	Brick activeBrick = PullBrickFromQueue(&brickQueue);
	Brick previewBrick = PullBrickFromQueue(&brickQueue);
	previewBrick.SetPreviewPosition();

	bool isGameOver = false;

	while (renderWindow.IsOpen())
	{
		switch (state)
		{
			case GameState::MainMenu:
			{
				while (renderWindow.PollEvent(gameEvent))
				{
					if (gameEvent.Type == Event::Closed || gameEvent.Key.Code == Keyboard::Escape)
					{
						renderWindow.Close();
					}
					else if (gameEvent.Type == Event::MouseButtonPressed)
					{
						Vector2i mousePos = Mouse::GetPosition(renderWindow);

						if (mousePos.x >= 220 && mousePos.x <= 420 &&
							mousePos.y >= 200 && mousePos.y <= 260)
						{
							state = GameState::Playing;
						}
						else if ((mousePos.x >= 220 && mousePos.x <= 420 &&
								 mousePos.y >= 300 && mousePos.y <= 360) ||
								 gameEvent.Key.Code == Keyboard::Escape)
						{
							renderWindow.Close();
						}
					}
				}

				renderWindow.Draw(mainMenuBackgroundSprite);
				renderWindow.Draw(mainMenuHeaderText);
				renderWindow.Draw(mainMenuStartText);
				renderWindow.Draw(mainMenuQuitText);
				renderWindow.Display();
				break;
			}
			
			case GameState::GameOver:
			{
				Text gameOverScoreHeaderText = scoreHeaderText;
				gameOverScoreHeaderText.SetPosition(130, 320);
				Text gameOverScoreDisplayText = scoreDisplayText;
				gameOverScoreDisplayText.SetPosition(210, 320);
				Text gameOverLevelHeaderText = levelHeaderText;
				gameOverLevelHeaderText.SetPosition(130, 350);
				Text gameOverLevelDisplayText = levelDisplayText;
				gameOverLevelDisplayText.SetPosition(210, 350);
				
				while (renderWindow.PollEvent(gameEvent))
				{
					if (gameEvent.Type == Event::Closed || gameEvent.Key.Code == Keyboard::Escape)
					{
						renderWindow.Close();
					}
				}

				renderWindow.Draw(gameOverBackgroundSprite);
				renderWindow.Draw(gameOverHeaderText);
				renderWindow.Draw(gameOverScoreHeaderText);
				renderWindow.Draw(gameOverScoreDisplayText);
				renderWindow.Draw(gameOverLevelHeaderText);
				renderWindow.Draw(gameOverLevelDisplayText);
				renderWindow.Display();
				break;
			}

			case GameState::Playing:
			{
				if (activeBrick.IsLocked())
				{
					dropSound.Play();
					brickList.push_back(activeBrick);
					brickCount[(int)activeBrick.GetBrickType()]++;
					int linesCleared = 0;

					for (int i = 0; i < SPRITES_IN_BRICK; i++)
					{	
						if (activeBrick.GetSpriteYPosition(i) < BRICK_UPPER_BOUNDRY)
						{
							state = GameState::GameOver;
							break;
						}
						else
						{
							gameBoard[activeBrick.GetSpriteYPosition(i)][activeBrick.GetSpriteXPosition(i)] = true;
						}
					}

					if (state == GameState::GameOver)
					{
						break;
					}

					//Check if there are any full rows that need to be cleared.
					for (int i = BRICK_UPPER_BOUNDRY; i < GB_HEIGHT; i++)
					{
						for (int j = 0; j < GB_WIDTH; j++)
						{
							if (gameBoard[i][j])
							{
								if (j == GB_WIDTH - 1)
								{
									ClearRow(&brickList, i, gameBoard);
									linesCleared++;
									totalLines++;

									if (totalLines % 8 == 0 && level < LEVEL_MAX)
									{
										level++;
									}
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

					if (linesCleared > 0)
					{
						if (linesCleared > 1 && linesCleared < 4)
						{
							score += (linesCleared * MULTI_LINE_CLEAR_SCORE) + BRICK_LOCK_SCORE;
						}
						else if (linesCleared == 4)
						{
							score += TETRIS_CLEAR_SCORE + BRICK_LOCK_SCORE;
						}
						else
						{
							score += SINGLE_LINE_CLEAR_SCORE + BRICK_LOCK_SCORE;
						}
					}
					else
					{
						score += BRICK_LOCK_SCORE;
					}
	
					activeBrick = previewBrick;
					activeBrick.SetActivePosition();
					previewBrick = PullBrickFromQueue(&brickQueue);
					previewBrick.SetPreviewPosition();
				}

				else
				{
					Time fallTimer = brickFallTimer.GetElapsedTime();
					Time lockTimer = brickLockTimer.GetElapsedTime();
					float dropSpeed = 1 - (float)(level * 0.05f);

					if (!IsBottomCollision(activeBrick, gameBoard) && fallTimer.AsSeconds() >= dropSpeed)
					{
						for (int i = 0; i < SPRITES_IN_BRICK; i++)
						{
							activeBrick.SetSpriteYPosition(i, 1);
							brickFallTimer.Restart();
							isCheckingLockTime = false;
						}
					}

					else if (IsBottomCollision(activeBrick, gameBoard))
					{					
						if (!isCheckingLockTime)
						{
							isCheckingLockTime = true;
							brickLockTimer.Restart();
						}

						else if (lockTimer.AsSeconds() >= 0.5f)
						{
							activeBrick.LockBrick();
							isCheckingLockTime = false;
						}
					}
			
				}

				while (renderWindow.PollEvent(gameEvent))
				{
					if (gameEvent.Type == Event::Closed || gameEvent.Key.Code == Keyboard::Escape)
					{
						renderWindow.Close();
					}

					if (gameEvent.Type == Event::KeyPressed)
					{
						CheckInput(&gameEvent, &activeBrick, gameBoard, &score);
					}
				}
		
				scoreDisplayText.SetString(_itoa(score, scoreDisplayBuffer, 10));
				levelDisplayText.SetString(_itoa(level, levelDisplayBuffer, 10));

				for (int i = 0; i < NUMBER_OF_BRICK_TYPES; i++)
				{
					brickCountText[i].SetString(_itoa(brickCount[i], brickCountTextBuffer[i], 10));
				}
				
				renderWindow.Clear(Color(0, 255, 255));
				renderWindow.Draw(backgroundSprite);
				renderWindow.Draw(scoreHeaderText);
				renderWindow.Draw(scoreDisplayText);
				renderWindow.Draw(levelHeaderText);
				renderWindow.Draw(levelDisplayText);
				renderWindow.Draw(brickPreviewSprite);
				renderWindow.Draw(nextBrickText);

				for (int i = 0; i < NUMBER_OF_BRICK_TYPES; i++)
				{
					renderWindow.Draw(brickCountText[i]);
				}

				for (int i = 0; i < brickList.size(); i++)
				{
					for (int j = 0; j < SPRITES_IN_BRICK; j++)
					{
						renderWindow.Draw(brickList[i].GetSpriteArray()[j]);
					}
				}
		
				for (int i = 0; i < SPRITES_IN_BRICK; i++)
				{
					renderWindow.Draw(activeBrick.GetSpriteArray()[i]);
					renderWindow.Draw(previewBrick.GetSpriteArray()[i]);
				}
				renderWindow.Display();
				break;

			}	//END CASE

		}	//END SWITCH

	} //END GAME LOOP
	
	return 0;
}