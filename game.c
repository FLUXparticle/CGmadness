/*
 * CG Madness - a Marble Madness clone
 * Copyright (C) 2007  Sven Reinck <sreinck@gmail.com>
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
 */

#include "game.h"

#include "common.h"
#include "level.h"
#include "highscore.h"

#include "objects.h"
#include "ball.h"
#include "field.h"
#include "menumanager.h"
#include "gamemenu.h"
#include "files.h"
#include "features.h"
#include "keyboard.h"
#include "camera.h"
#include "callback.h"
#include "text.hpp"
#include "texture.h"
#include "environment.h"
#include "noise.h"
#include "atlas.h"
#include "main.h"

#include "stringlist.h"

#include "color.h"

#include "functions.h"

#include "types.h"
#include "debug.h"

#include <GL/glew.h>
#include <GL/glut.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#define FOG_DENSITY 0.003f

int sgRenderPass = 8;

static int gIsGameRunning;

static float gGameTime;

static const char *gHotSeatLevel = NULL;

void pauseGame(void)
{
	disableBallCamera();
	gIsGameRunning = 0;
}

void resumeGame(void)
{
	enableBallCamera();
	gIsGameRunning = 1;
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
		int i = 0;

		if (wasKeyPressed(KEY_ESC))
		{
			pauseGame();
			showGameMenu(1);
		}

		/* for some debug */
		for (i = 0; i < 9; i++)
		{
			if (wasKeyPressed('1' + i))
			{
				sgRenderPass = i;
			}
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
#if NOISE_TEXTURE
		if (sgLevel.colorMap == 0 && !sgLevel.waiting)
		{
			sgLevel.colorMap = genTexture();
			colorMapToTexture(sgLevel.colorMap);
			resetBall();
		}
#endif

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
	drawGameField(0);
	drawGameBall();
}

void drawGameBallReflection(void)
{
	drawEnvironment(drawGameWaterReflection);
	drawGameField(1);
}

void drawGame(void)
{
	drawEnvironment(drawGameWaterReflection);

	drawGameField(0);
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

void startGame(void)
{
	sgLevel.lightMap = genTexture();
	lightMapToTexture(sgLevel.lightMap);

	updateTexCoords();

#if (NOISE_TEXTURE)
	updateColorMap();

	pushWaitScreen();

	sgLevel.colorMap = 0;
#endif

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
#if (NOISE_TEXTURE)
	glDeleteTextures(1, &sgLevel.colorMap);
#endif

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
