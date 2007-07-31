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

#include "gamemenu.h"

#include "text.h"
#include "graph.h"
#include "objects.h"
#include "texture.h"
#include "ball.h"
#include "game.h"
#include "features.h"
#include "keyboard.h"
#include "gui.h"
#include "camera.h"
#include "debug.h"

#include <GL/glut.h>

#include <stdio.h>
#include <string.h>

#define SCALE_FONT 0.5f

typedef struct {
	char* left;
	char* right;
} LeftRight;

static int gResume;

static Object goLogo;

static Check gcShadows;
static Check gcReflection;

/*
 * help text
 */
static LeftRight gTextHelp[] = {
	{ "Cursor", "Move" },
	{ "Space", "Jump" },
	{ "W A S D", "Camera" },
	{ "R F", "Zoom" },
	{ "Enter", "Reset" },
	{ "Esc", "Menu" },
};

static Vector3 gGameMenuPosition;

Menu gMenuMain1;
Menu gMenuMain2;
Menu gMenuHelp;
Menu gMenuNext;
Menu gMenuEnd;

Menu* gCurMenu;

/* events */

int gCntBallLayouts;
int gBallLayouts[MAX_BALL_LAYOUTS];

static void clickButtonStart(void) {
	resumeGame();
}

static void changeBallEdit(void* self) {
	changeBall(gBallLayouts[((SpinEdit*) self)->value]);
}

static void changeShadows(void* self) {
	Check* check = self;
	setShadows(check->value);
}

static void changeReflection(void* self) {
	Check* check = self;
	setReflection(check->value);
}

static void clickButtonHelp(void) {
	gCurMenu = &gMenuHelp;
}

static void clickButtonQuit(void) {
	exit(0);
}

static void clickButtonAgain(void) {
	resetGame();
}

static void clickButtonBack(void) {
	/*
	 * TODO save last menu
	 */
	gCurMenu = &gMenuMain1;
}

void updateGameMenu(float interval) {
	Vector3 camera = gGameMenuPosition;
	Vector3 lookat = gGameMenuPosition;

	camera.y -= 10.0f;
	camera.z += 7.0f;

	lookat.z += 5.0f;

	moveCamera(interval, camera, lookat);

	updateMenu(gCurMenu, interval);

	if (gCurMenu == &gMenuMain1 || gCurMenu == &gMenuMain2)
	{
		if (wasKeyPressed(KEY_ENTER)) {
			clickButtonStart();
		}

		if (gResume && wasKeyPressed(KEY_ESC)) {
			clickButtonStart();
		}

		if (wasKeyPressed('h')) {
			clickButtonHelp();
		}

		if (wasKeyPressed('q')) {
			clickButtonQuit();
		}
	}
	else if (gCurMenu == &gMenuNext)
	{
		if (wasKeyPressed(KEY_ENTER)) {
			clickButtonStart();
		}

		if (wasKeyPressed(KEY_ESC)) {
			clickButtonBack();
		}
	}
	else if (gCurMenu == &gMenuHelp)
	{
		if (wasKeyPressed(KEY_ESC)) {
			clickButtonBack();
		}
	}
	else if (gCurMenu == &gMenuEnd)
	{
		if (wasKeyPressed(KEY_ENTER)) {
			clickButtonAgain();
		}
		if (wasKeyPressed(KEY_ESC) || wasKeyPressed('q')) {
			clickButtonQuit();
		}
	}
}

void setGameMenuPosistion(Vector3 pos) {
	gGameMenuPosition = pos;
}

void drawGameMenu(void) {
	glEnable(GL_LIGHTING);

		setSomeLight();

		glPushMatrix();
			glTranslatef(gGameMenuPosition.x, gGameMenuPosition.y, gGameMenuPosition.z);
			glRotatef(90.0f, 1.0f, 0.0f, 0.0f);

			drawObject(&goLogo);
			drawMenu(gCurMenu);
		glPopMatrix();

	glDisable(GL_LIGHTING);
}

void pickGameMenu(const Vector3* position, const Vector3* direction, MouseEvent event) {
	Vector3 newPosition = sub(*position, gGameMenuPosition);

	if (newPosition.y < 0.0f && direction->y > 0.0f)
	{
		float t = -newPosition.y / direction->y;
		float x = newPosition.x + t * direction->x;
		float y = newPosition.z + t * direction->z;

		clickMenu(gCurMenu, x, y, event);
	}
}

void showGameMenu(int menu) {
	gResume = menu != 0;

	switch (menu) {
	case 0:
		gCurMenu = &gMenuMain1;
		break;
	case 1:
		gCurMenu = &gMenuMain2;
		break;
	case 2:
		gCurMenu = &gMenuNext;
		break;
	case 3:
		gCurMenu = &gMenuEnd;
		break;
	}

	setCheck(&gcShadows, useShadows());
	setCheck(&gcReflection, useReflection());

	glutSetCursor(GLUT_CURSOR_LEFT_ARROW);
}

void initGameMenu() {
	static Button bStart;
	static Button bResume;
	static Button bQuit;
	static Button bHelp;
	static Button bBack;
	static Button bContinue;
	static Button bMain;
	static Button bAgain;
	static Button bQuit2;

	static SpinEdit spinEditBall;

	static Object oTextHelp[2 * LENGTH(gTextHelp)];
	static Object oBall;

	static MenuItem* itemsMain1[] =
	{
		&bStart.item,
		&bHelp.item,
		&bQuit.item
	};

	static MenuItem* itemsMain2[] =
	{
		&bResume.item,
		&bHelp.item,
		&bQuit.item
	};

	static MenuItem* itemsNext[] =
	{
		&bContinue.item,
		&bMain.item
	};

	int i;

	initGUI();

	/*
	 * which ball layouts are available?
	 */
	gCntBallLayouts = 0;

	gBallLayouts[gCntBallLayouts++] = BALL_LAYOUT_DEFAULT;

	if (hasBallTexture()) {
		gBallLayouts[gCntBallLayouts++] = BALL_LAYOUT_TEXTURE;
	}

	if (hasBallReflection()) {
		gBallLayouts[gCntBallLayouts++] = BALL_LAYOUT_METAL;
	}

	if (hasBallShader()) {
		gBallLayouts[gCntBallLayouts++] = BALL_LAYOUT_GOLFBALL;
	}

	if (hasBallShader() && hasBallReflection()) {
		gBallLayouts[gCntBallLayouts++] = BALL_LAYOUT_GOLFBALL_METAL;
	}

	/*
	 * put all together
	 */

	/* menu logo */
	initObject(&goLogo, drawSquare);
	goLogo.texture = loadTexture("data/logo.tga", 0);
	setObjectPosition3f(&goLogo, 0.0f, 8.0f, 0.0f);
	setObjectScale3f(&goLogo, 4.0f, 1.0f, 1.0f);

	/* main menu */
	initObject(&oBall, drawMenuBall);

	initButton(&bStart, 6.0f, clickButtonStart, "Start");
	initButton(&bResume, 6.0f, clickButtonStart, "Resume");

	init3dSpinEdit(&spinEditBall, gCntBallLayouts - 1, 0, gCntBallLayouts - 1, 5.2f, &oBall, changeBallEdit);
	init3dCheck(&gcShadows, 4.0f, changeShadows, "Shadows");
	init3dCheck(&gcReflection, 3.0f, changeReflection, "Reflection");

	initButton(&bHelp, 2.0f, clickButtonHelp, "Help");
	initButton(&bQuit, 1.0f, clickButtonQuit, "Quit");

	INIT_MENU(&gMenuMain1, itemsMain1);
	INIT_MENU(&gMenuMain2, itemsMain2);

	/* next level menu */
	initButton(&bContinue, 5.5f, clickButtonStart, "Continue");
	initButton(&bMain, 4.5f, clickButtonBack, "Main Menu");

	INIT_MENU(&gMenuNext, itemsNext);

	/* help menu */
	for (i = 0; i < LENGTH(gTextHelp); i++) {
		float z = 6.0f - i;
		float length;

		{
			Object* o = &oTextHelp[2 * i];

			length = makeTextObject(o, gTextHelp[i].left) * SCALE_FONT;
			setObjectPosition3f(o, -5.0f, 0.0f, z);
			setObjectScalef(o, SCALE_FONT);
			rotateObjectX(o, 90.0f);
		}

		{
			Object* o = &oTextHelp[2 * i + 1];

			length = makeTextObject(o, gTextHelp[i].right) * SCALE_FONT;
			setObjectPosition3f(o, 5.0f - length, 0.0f, z);
			setObjectScalef(o, SCALE_FONT);
			rotateObjectX(o, 90.0f);
		}
	}

	initButton(&bBack, 6.0f - LENGTH(gTextHelp), clickButtonBack, "back");

	/* game complete menu */
	initButton(&bAgain, 5.5f, clickButtonAgain, "Play Again");
	initButton(&bQuit2, 4.5f, clickButtonQuit, "Quit");
}
