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
#include "light.h"
#include "objects.h"
#include "ball.h"
#include "shadows.h"
#include "field.h"
#include "menu.h"
#include "files.h"
#include "features.h"
#include "keyboard.h"

#include "types.h"
#include "debug.h"

#include <GL/glew.h>
#include <GL/glut.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

int sgRenderPass = 8;

int sgGameMainLight;
int sgGameSpotLight;

static int gIsGameRunning;

static Object goMenu;

void pauseGame(void) {
	showMenu(1);
	gIsGameRunning = 0;
	goMenu.visible = 1;
}

void resumeGame(void) {
	glutSetCursor(GLUT_CURSOR_NONE);
	gIsGameRunning = 1;
	goMenu.visible = 0;
}

void updateGameCamera(float interval, Vector3 ball) {
	Vector3 diff;
	Vector3 up = { 0.0f, 0.0f, 1.0f };
	static float distance = 5.0f;
	static float height = 2.0f;
	static Vector3 dest = { 0.0f, 0.0f, 0.0f };

  /* game controls for camera */

	/* zoom */
	if (isKeyPressed('f')) distance += 0.1f;
	if (isKeyPressed('r') && distance > 0.5) distance -= 0.1f;

	/* rotation */
	if (isKeyPressed('a')) dest = sub(dest, scale(0.1f, sgRight));
	if (isKeyPressed('d')) dest = add(dest, scale(0.1f, sgRight));

	/* height */
	if (isKeyPressed('w')) height += 0.1f;
	if (isKeyPressed('s')) height -= 0.1f;

	dest.z = ball.z + height;

	diff = sub(dest, ball);
	diff = scale(distance, norm(diff));

	dest = add(ball, diff);

	moveCamera(interval, dest, ball);

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
			toggleLight(sgGameSpotLight);
		}
		if (wasFunctionPressed(2)) {
			setShadows(!useShadows());
		}

		updateBall(interval);

		sgLight[sgGameSpotLight].pos[0] = sgoBall.pos.x;
		sgLight[sgGameSpotLight].pos[1] = sgoBall.pos.y;
		sgLight[sgGameSpotLight].pos[2] = sgoBall.pos.z;

		updateShadows();

		updateGameCamera(interval, sgoBall.pos);
	} else {
		/* show menu */
		Vector3 camera;
		Vector3 lookat = goMenu.pos;
		
		lookat.x = (sgLevel.size.x + 1.0f) / 2;
		lookat.z += 5.0f;

		camera = lookat;
		camera.y -= 10.0f;
		camera.z += 2.0f;

		updateMenu(interval);

		moveCamera(interval, camera, lookat);
	}
	
	updateGameField();
}

void drawGameContent(int reflection) {
	/*
	 * WARNING: alpha blending does not seem to work in texture-buffer
	 */
	drawGameField();
	drawShadows(!reflection);
	
	if (goMenu.visible)	{
		drawObject(&goMenu);
	}
}

void drawGame(void) {
	drawGameContent(0);
}

void drawGameReflection(void) {
	drawGameContent(1);
}

void pickGame(void) {
	if (goMenu.visible)	{
		pickObject(&goMenu);
	}
}

void initLevel(char* filename) {
	loadFieldFromFile(filename);
	
	initGameField();

	initShadowVolumes();

	sgCamera.x = -2.0f;
	sgCamera.y = -3.0f;
	sgCamera.z =  4.0f;

	setObjectPosition3f(&goMenu, (sgLevel.size.x + 1.0f) / 2, -10.0f, 0.0f);

	sgLight[sgGameSpotLight].pos[0] = sgLevel.start.x + 0.5f;
	sgLight[sgGameSpotLight].pos[1] = sgLevel.start.y + 0.5f;
	
	updateGameCamera(0.0, goMenu.pos);

	showMenu(0);
	gIsGameRunning = 0;
	goMenu.visible = 1;

	resetBall();
}

void destroyLevel(void) {
	destroyGameField();
	destroyCommon();
	destroyShadowVolumes();
}

char* getNextLevelName(void) {
	static char* allLevels = NULL;
	static char* nextLevel = NULL;
	static char* curLevel = NULL;

	if (!allLevels) {
		allLevels = textFileRead("levels/default.lev");
		nextLevel = allLevels;
	}

	curLevel = nextLevel;

	nextLevel = strchr(curLevel, '\n');

	if (nextLevel) {
		*nextLevel = '\0';
		nextLevel++;
		return curLevel;
	} else {
		return NULL;
	}
}

void loadNewLevel(void) {
	char* nextLevelname = getNextLevelName();

	destroyLevel();

	if (nextLevelname) {
		initLevel(nextLevelname);
	} else {
		exit(0);
	}
}

void initFog(void) {
	float value;
	int ivalue;

	ivalue = GL_LINEAR;
	glFogiv(GL_FOG_MODE, &ivalue);
	value = FOG_START;
	glFogfv(GL_FOG_START, &value);
	value = FOG_END;
	glFogfv(GL_FOG_END, &value);
}

void initGame(char* filename) {
	initObjects();

	glColor3f(1.0f, 1.0f, 1.0f);
	sgGameMainLight = addPointLight(-200.0f, -200.0f, 100.0f);
	glColor3f(1.0f, 1.0f, 0.0f);
  sgGameSpotLight = addSpotLight(0.0f, 0.0f, 8.0f, 0.0f, 0.0f, -1.0f, 10.0f, 50.0f);

	initFog();
	
	/* ball */
	initBall();

	/* menu (must be after ball) */
	initMenu(&goMenu);

	goMenu.visible = 0;
	gIsGameRunning = 1;

	/* level (must be after menu) */
 	initLevel(getNextLevelName());

	sgWindowViewport.draw = drawGame;
	sgWindowViewport.pick = pickGame;
	setUpdateFunc(updateGame);
}

