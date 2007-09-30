/*
 * CG Madness - a Marble Madness clone
 * Copyright (C) 2007  Sven Reinck <sreinck@gmx.de>
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

#include "objects.h"
#include "ball.h"
#include "field.h"
#include "menumanager.h"
#include "gamemenu.h"
#include "files.h"
#include "features.h"
#include "keyboard.h"
#include "mouse.h"
#include "camera.h"
#include "callback.h"
#include "environment.h"
#include "text.h"
#include "texture.h"
#include "noise.h"
#include "atlas.h"

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

#if (MOUSE_CONTROL)
static int gDragX = 0;
static int gDragY = 0;
#endif

static float gDistance;
static float gLatitude;
static float gLongitude;

#if (MOUSE_CONTROL)
void gameDrag(int dx, int dy) {
	gDragX += dx;
	gDragY += dy;
}
#endif

void pauseGame(void) {
#if (MOUSE_CONTROL)
	setDragFunc(NULL);
#endif
	showGameMenu(1);
	gIsGameRunning = 0;
}

void resumeGame(void) {
#if (MOUSE_CONTROL)
	setDragFunc(gameDrag);
#endif
	glutSetCursor(GLUT_CURSOR_NONE);
	gIsGameRunning = 1;
}

void updateGameCamera(float interval, Vector3 ball) {
	Vector3 diff;
	Vector3 up = { 0.0f, 0.0f, 1.0f };
	static Vector3 dest = { 0.0f, 0.0f, 0.0f };

  /* game controls for camera */

#if (MOUSE_CONTROL)
	gLongitude -= 5.0f * interval * gDragX;
	gLatitude += 5.0f * interval * gDragY;

	gDragX = 0;
	gDragY = 0;
#else
	/* zoom */
	if (isKeyPressed('f') && gDistance < 20.0f) gDistance += 0.1f;
	if (isKeyPressed('r') && gDistance > 0.5) gDistance -= 0.1f;

	/* rotation */
	if (isKeyPressed('a')) gLongitude -= 120.0f * interval;
	if (isKeyPressed('d')) gLongitude += 120.0f * interval;

	/* height */
	if (isKeyPressed('w')) gLatitude += 120.0f * interval;
	if (isKeyPressed('s')) gLatitude -= 120.0f * interval;

	gLatitude = clamp(gLatitude, -89.0f, 89.0f);
#endif

	dest.x = ball.x + gDistance * sin(gLongitude * PI / 180.0f) * cos(gLatitude * PI / 180.0f);
	dest.y = ball.y - gDistance * cos(gLongitude * PI / 180.0f) * cos(gLatitude * PI / 180.0f);
	dest.z = ball.z + gDistance * sin(gLatitude * PI / 180.0f);

	moveCamera(interval, dest, ball);

#if (MOUSE_CONTROL)
	sgCamera = dest;
#endif

	diff = sub(sgLookat, sgCamera);
	diff.z = 0.0f;
	sgForward = norm(diff);
	sgRight = norm(cross(sgForward, up));
}

void updateGame(float interval) {
	if (gIsGameRunning) {
		int i = 0;

		if (wasKeyPressed(KEY_ESC)) {
			pauseGame();
		}

		/* for some debug */
		for (i = 0; i < 9; i++) {
			if (wasKeyPressed('1' + i)) {
				sgRenderPass = i;
			}
		}

		/* manually switch features */
		if (wasFunctionPressed(1)) {
			setSpotlight(!useSpotlight());
		}
		if (wasFunctionPressed(2)) {
			setShadows(!useShadows());
		}

		if (wasFunctionPressed(3)) {
			setReflection(!useReflection());
		}

		updateBall(interval);

		updateGameCamera(interval, sgoBall.pos);

		if (!sgIsBallInPieces)
		{
			gGameTime += interval;
		}
	} else {
		if (updateMenuManager(interval))
		{
			gIsGameRunning = 1;
		}
	}

	if (sgLevel.colorMap == 0 && !sgLevel.waiting)
	{
		sgLevel.colorMap = genTexture();
		colorMapToTexture(sgLevel.colorMap);
	}


	updateGameField();

	updateEnvironment(interval);
}

void drawGameHUD(float widthWindow, float heightWindow)
{
	int seconds = (int) gGameTime;
	float scale = 0.06f;
	float widthDefault = widthStrokeText("x:xx.x") * scale;

	char strTime[10];
	float width;
	float height;

	sprintf(strTime, "%d:%02d.%01d",  seconds / 60, seconds % 60, (int) ((gGameTime - seconds) * 10.0f));

	width = widthStrokeText(strTime) * scale;
	height = scale;

	glColor3f(1.0f, 1.0f, 0.0f);

	glPushMatrix();

		glTranslatef((widthWindow - widthDefault) / 2.0f, (heightWindow - height), 0.0f);
		glScalef(scale, scale, scale);

		drawStrokeText(strTime);

	glPopMatrix();
}

void drawGame(void) {
	/*
	 * WARNING: alpha blending does not seem to work in texture-buffer
	 */
	drawEnvironment();
	drawGameField(0);
	drawGameBall();

	if (!gIsGameRunning)	{
		drawMenuManager();
	}
}

void drawGameReflection(void) {
	drawEnvironment();
	drawGameField(1);
}

void eventGame(const Vector3* position, const Vector3* direction, MouseEvent event)
{
	if (!gIsGameRunning)
	{
		eventMenuManager(position, direction, event);
	}
}

void resetGameTime(void)
{
	gGameTime = 0.0f;
}

static int startLevel(const char* filename) {
	static Vector3 gameMenuPosition;

	if (!loadFieldFromFile(filename)) {
		return 0;
	}

	if (sgLevel.borderTexture == 0) {
#if (NOISE_TEXTURE)
		sgLevel.borderTexture = loadTexture("data/boarder.tga", 1);
#else
		sgLevel.borderTexture = loadTexture("data/plate.tga", 1);
#endif
	}

	sgLevel.lightMap = genTexture();
	lightMapToTexture(sgLevel.lightMap);

	updateTexCoords();

#if (NOISE_TEXTURE)
	updateColorMap();

	showWaitScreen();

	sgLevel.colorMap = 0;
#endif

	initGameField();

	gDistance  =  5.0f;
	gLatitude  = 20.0f;
	gLongitude =  0.0f;

	gameMenuPosition.x = sgLevel.size.x / 2.0f;
	gameMenuPosition.y = -10.0f;
	gameMenuPosition.z =   0.0f;

	setMenuPosistion(gameMenuPosition);

	resetGameTime();

	return 1;
}

static char* getNextLevelName(void) {
	static char* allLevels = NULL;
	static char* nextLevel = NULL;
	static char* curLevel = NULL;

	if (!allLevels) {
		allLevels = textFileRead("levels/default.lev");
		nextLevel = allLevels;
	} else if (nextLevel != allLevels) {
		*nextLevel = '\n';
		nextLevel++;
	}

	curLevel = nextLevel;

	nextLevel = strchr(curLevel, '\n');

	if (nextLevel) {
		*nextLevel = '\0';
		return curLevel;
	} else {
		nextLevel = allLevels;
		return NULL;
	}
}

void loadNewLevel(void) {
	char* nextLevelname = getNextLevelName();

	if (!nextLevelname) {
		pauseGame();
		showGameMenu(3);
	} else {
		glDeleteTextures(1, &sgLevel.lightMap);
#if (NOISE_TEXTURE)
		glDeleteTextures(1, &sgLevel.colorMap);
#endif
	
		destroyGameField();
		destroyLevel();

		if (startLevel(nextLevelname)) {
			pauseGame();
			showGameMenu(0);
			showGameMenu(2);
			resetBall();
		} else {
			exit(1);
		}
	}
}

void resetGame(void) {
	loadNewLevel();
	showGameMenu(0);
	updateGameField();
	resetCamera();
}

void initFog(void) {
	int mode = GL_EXP;
	float density = FOG_DENSITY;
	float color[] = {1.0f, 1.0f, 1.0f, 1.0f};

	glEnable(GL_FOG);
	glFogiv(GL_FOG_MODE, &mode);
	glFogfv(GL_FOG_DENSITY, &density);
	glFogfv(GL_FOG_COLOR, color);
}

int initGame(void) {
	resetCamera();

	initObjects();

	initFog();

	/* ball */
	initBall();

	/* menu (must be after ball) */
	initGameMenu();

	/* level (must be after menu) */
 	if (!startLevel(getNextLevelName())) {
		return 0;
	}

	initEnvironment();

	gIsGameRunning = 1;

	pauseGame();
	showGameMenu(0);
	resetBall();

	updateGameField();

	sgWindowViewport.draw = drawGame;
	sgWindowViewport.drawHUD = drawGameHUD;
	sgWindowViewport.mouseEvent = eventGame;
	setUpdateFunc(updateGame);

	return 1;
}
