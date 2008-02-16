#include "Game.hpp"

#include "common.hpp"
#include "level.hpp"
#include "highscore.hpp"

#include "objects.hpp"
#include "ball.hpp"
#include "field.hpp"
#include "menumanager.hpp"
#include "gamemenu.hpp"
#include "features.hpp"
#include "keyboard.hpp"
#include "camera.hpp"
#include "callback.hpp"
#include "text.hpp"
#include "texture.hpp"
#include "environment.hpp"
#include "atlas.hpp"
#include "main.hpp"

#include "stringlist.hpp"

#include "color.hpp"

#include "functions.hpp"

#include "types.hpp"

#include <GL/glew.h>
#include <GL/glut.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#define FOG_DENSITY 0.003f

Game::Game()
{
  // empty
}

Game::~Game()
{
  // empty
}

void Game::init()
{
	initGame();
}

void Game::start()
{
	startGame();
}

void Game::update(float interval)
{
	updateGame(interval);
}

void Game::draw(void)
{
	drawGame();
}

void Game::drawHUD(float width, float height)
{
	drawGameHUD(width, height);
}

static bool gIsGameRunning;

static float gGameTime;

static const char *gHotSeatLevel = NULL;

void pauseGame(void)
{
	disableBallCamera();
	gIsGameRunning = false;
}

void resumeGame(void)
{
	enableBallCamera();
	gIsGameRunning = true;
}

void setHotSeatLevel(const char *filename)
{
	gHotSeatLevel = filename;
}

void stopWatch(void)
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

void finishedGame()
{
	stopWatch();
	pauseGame();
	showGameMenu(2);
}

void updateGame(float interval)
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
			sgIsMouseControl = !sgIsMouseControl;
			resumeGame();
		}

		if (!sgIsBallInPieces)
		{
			gGameTime += interval;
		}

		updateBall(interval);

		if (sgHasBallHitGoal)
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

void drawGameHUD(float widthWindow, float heightWindow)
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

void drawGame(void)
{
	drawEnvironment(drawGameWaterReflection);

	drawGameField(false);
	drawGameBall();

	if (!gIsGameRunning)
	{
		drawMenuManager();
	}
}

void resetGameTime(void)
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

void startGame(void)
{
	sgLevel.lightMap = genTexture();
	lightMapToTexture(sgLevel.lightMap);

	updateTexCoords();

	initGameField();

	resetGame();
}

#if 0
static const char *getNextLevelName(void)
{
	if (gHotSeatLevel)
	{
		if (gInGame)
		{
			return NULL;
		}
		else
		{
			return gHotSeatLevel;
		}
	}
	else
	{
		if (gNextLevelIndex < gLevelNames.count)
		{
			char *name = gLevelNames.strings[gNextLevelIndex];
			gNextLevelIndex++;
			return name;
		}
		else
		{
			gNextLevelIndex = 0;
			return NULL;
		}
	}
}
#endif

void stopGame(void)
{
	glDeleteTextures(1, &sgLevel.lightMap);

	destroyGameField();
}

void resetGame(void)
{
	resetBall();
	resetBallCamera();

	resetGameTime();

	updateGameField();

	pauseGame();
	showGameMenu(0);
}

void initFog(void)
{
	int mode = GL_EXP;
	float density = FOG_DENSITY;
	float color[] = { 1.0f, 1.0f, 1.0f, 1.0f };

	glEnable(GL_FOG);
	glFogiv(GL_FOG_MODE, &mode);
	glFogfv(GL_FOG_DENSITY, &density);
	glFogfv(GL_FOG_COLOR, color);
}

void initGame(void)
{
	resetCamera();

	initObjects();

	initFog();

	/* ball */
	initBall();

	/* menu (must be after ball) */
	initGameMenu();

#if 0
	/* level (must be after menu) */
	if (!startGame(getNextLevelName()))
	{
		return 0;
	}

	gIsGameRunning = 0;
	showGameMenu(0);
	resetBall();

	updateGameField();
#endif
}
