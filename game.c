/*
 * CG Madness - a Marble Madness clone
 * Copyright (C) 2007  Sven Reinck
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
 *
 * $Id$
 *
 */

#include "game.h"

#include "common.h"

#include "graph.h"
#include "objects.h"
#include "ball.h"
#include "field.h"
#include "gamemenu.h"
#include "files.h"
#include "features.h"
#include "keyboard.h"
#include "mouse.h"
#include "callback.h"
#include "environment.h"

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

#if (MOUSE_CONTROL)
static int gDragX = 0;
static int gDragY = 0;
#endif

static float gDistance;
static float gLatitude;
static float gLongitude;

static Vector3 gGameMenuPosistion;

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

		updateBall(interval);

		updateGameCamera(interval, sgoBall.pos);
	} else {
		/* show menu */
		Vector3 camera = gGameMenuPosistion;
		Vector3 lookat = gGameMenuPosistion;

		camera.y -= 10.0f;
		camera.z += 7.0f;

		lookat.z += 5.0f;

		updateGameMenu(interval);

		moveCamera(interval, camera, lookat);
	}

	updateGameField();

	updateEnvironment(interval);
}

void drawGame(void) {
	/*
	 * WARNING: alpha blending does not seem to work in texture-buffer
	 */
	drawEnvironment();
	drawGameField(0);
	drawGameBall();

	if (!gIsGameRunning)	{
		drawGameMenu();
	}
}

void drawGameReflection(void) {
#if 1
	drawEnvironment();
	drawGameField(1);
#endif
}

void pickGame(void) {
	if (!gIsGameRunning)	{
		pickGameMenu();
	}
}

int initLevel(const char* filename) {
	if (!loadFieldFromFile(filename)) {
		return 0;
	}

	initTextures();
	updateTextures(1);

	initGameField();

	gDistance  =  5.0f;
	gLatitude  = 20.0f;
	gLongitude =  0.0f;

	gGameMenuPosistion.x = sgLevel.size.x / 2.0f;
	gGameMenuPosistion.y = -10.0f;
	gGameMenuPosistion.z =   0.0f;

	setGameMenuPosistion(gGameMenuPosistion);

	updateGameCamera(0.0, gGameMenuPosistion);

	return 1;
}

void destroyLevel(void) {
	destroyGameField();
	destroyCommon();
}

char* getNextLevelName(void) {
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
		destroyLevel();
		if (initLevel(nextLevelname)) {
			pauseGame();
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
	initCommon();

	initObjects();

	initFog();

	/* ball */
	initBall();

	/* menu (must be after ball) */
	initGameMenu();

	/* level (must be after menu) */
 	if (!initLevel(getNextLevelName())) {
		return 0;
	}

	initEnvironment();

	gIsGameRunning = 1;

	pauseGame();
	showGameMenu(0);
	resetBall();

	updateGameField();

	sgWindowViewport.draw = drawGame;
	sgWindowViewport.pick = pickGame;
	setUpdateFunc(updateGame);

	return 1;
}
