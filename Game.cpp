#include "Game.hpp"

#include "common.hpp"
#include "level.hpp"
#include "highscore.hpp"

#include "ball.hpp"
#include "field.hpp"
#include "menumanager.hpp"
#include "gamemenu.hpp"
#include "features.hpp"
#include "keyboard.hpp"
#include "camera.hpp"
#include "text.hpp"
#include "texture.hpp"
#include "environment.hpp"
#include "atlas.hpp"

#include "functions.hpp"

#include <stdio.h>

void initGame(void);

Game::Game()
{
  // empty
}

Game::~Game()
{
  // empty
}

void Game::pauseGame()
{
	disableBallCamera();
	gIsGameRunning = false;
}

void Game::resumeGame()
{
	enableBallCamera();
	gIsGameRunning = true;
}

void Game::stopWatch()
{
	int i;

	int tenthSecond = (int) (gGameTime * 10.0f);
	int newIndex = sgLevel.cntScoreCols;

	while (newIndex > 0 && tenthSecond < sgLevel.scores[newIndex - 1].tenthSecond)
	{
		newIndex--;
	}

	sgLevel.cntScoreCols = min(sgLevel.cntScoreCols + 1, MAX_SCORE_COLS);

	for (i = sgLevel.cntScoreCols - 1; i > newIndex; i--)
	{
		sgLevel.scores[i] = sgLevel.scores[i - 1];
	}

	if (newIndex < MAX_SCORE_COLS)
	{
		sgLevel.scores[newIndex].name[0] = '\0';
		sgLevel.scores[newIndex].tenthSecond = tenthSecond;
	}

	sgLastPlayerIndex = newIndex;
}

void Game::finishedGame()
{
	stopWatch();
	pauseGame();
	showGameMenu(2);
}

void Game::update(float interval)
{
	updateEnvironment(interval);
	if (gIsGameRunning)
	{
		if (wasKeyPressed(KEY_ESC))
		{
			pauseGame();
			showGameMenu(1);
		}

		/* manually switch features */
		if (wasFunctionPressed(1))
		{
			setBallShadow(!useBallShadow());
		}

		if (wasFunctionPressed(2))
		{
			setReflection(!useReflection());
		}

		if (wasFunctionPressed(5))
		{
			pauseGame();
			toggleMouseControl();
			resumeGame();
		}

		if (!sgoBall.isInPieces())
		{
			gGameTime += interval;
		}

		updateBall(sgoBall, interval);

		if (sgoBall.hasHitGoal())
		{
			finishedGame();
		}
	}
	else
	{
		updateMenuManager(interval);
	}

	updateGameField();
}

void Game::preDisplay()
{
	sgoBall.updateReflection();
}

void Game::drawHUD(float widthWindow, float heightWindow)
{
	int tenthSecond = (int) (gGameTime * 10.0f);
	float scale = 0.06f;
	float widthDefault = widthStrokeText("x:xx.x") * scale;

	char strTime[10];
	float width;
	float height;

	sprintf(strTime, "%d:%02d.%01d", tenthSecond / 600, tenthSecond / 10 % 60,
					tenthSecond % 10);

	width = widthStrokeText(strTime) * scale;
	height = scale;

	glColor3f(1.0f, 1.0f, 0.0f);

	glPushMatrix();

	glTranslatef((widthWindow - widthDefault) / 2.0f, (heightWindow - height),
							 0.0f);
	glScalef(scale, scale, scale);

	drawStrokeThickText(strTime);

	glPopMatrix();
}

void drawGameWaterReflection(void)
{
	drawGameField(false);
	drawGameBall();
}

void drawGameBallReflection(void)
{
	drawEnvironment(drawGameWaterReflection);
	drawGameField(true);
}

void Game::draw()
{
	drawEnvironment(drawGameWaterReflection);

	drawGameField(false);
	drawGameBall();

	if (!gIsGameRunning)
	{
		drawMenuManager();
	}
}

void Game::resetGameTime()
{
	gGameTime = 0.0f;
}

void lightMapToTexture(unsigned int texID)
{
	unsigned int sizeX;
	unsigned int sizeY;
	const float* data;
	
	getAtlasInfo(&sizeX, &sizeY, &data);
	
	glBindTexture(GL_TEXTURE_2D, texID);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_LUMINANCE8, sizeX, sizeY, 0, GL_LUMINANCE, GL_FLOAT, data);
}

void Game::start()
{
	sgLevel.lightMap = genTexture();
	lightMapToTexture(sgLevel.lightMap);

	updateTexCoords();

	initGameField();

	resetGame();
}

void Game::stop(void)
{
	glDeleteTextures(1, &sgLevel.lightMap);

	destroyGameField();
}

void Game::resetGame()
{
	resetBall();
	resetBallCamera();

	resetGameTime();

	updateGameField();

	pauseGame();
	showGameMenu(0);
}

void Game::init()
{
	/* ball */
	initBall();

	/* menu (must be after ball) */
	initGameMenu();
}
