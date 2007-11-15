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

#include "gamemenu.h"

#include "text.h"
#include "objects.h"
#include "texture.h"
#include "ball.h"
#include "game.h"
#include "features.h"
#include "keyboard.h"
#include "gui.h"
#include "menu.h"
#include "camera.h"
#include "debug.h"
#include "common.h"

#include <GL/glut.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define SCALE_FONT 0.5f

typedef struct {
	char* left;
	char* right;
} LeftRight;

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
Menu gMenuWait;

Menu* gCurMenu = NULL;

void pushGameMenu(Menu* menu)
{
	menu->back = gCurMenu;
	gCurMenu = menu;

	showMenu(gCurMenu);
}

void popGameMenu(void)
{
	gCurMenu = gCurMenu->back;

	if (gCurMenu)
	{
		showMenu(gCurMenu);
	}
	else
	{
		resumeGame();
	}
}

/* events */

int gCntBallLayouts;
int gBallLayouts[MAX_BALL_LAYOUTS];

static void clickButtonHelp(void)
{
	pushGameMenu(&gMenuHelp);
}

static void clickButtonQuit(void)
{
	exit(0);
}

static void clickButtonAgain(void)
{
	resetGame();
}

static void clickButtonContinue(void)
{
	gCurMenu->back = NULL;
	popGameMenu();
}

static void clickButtonBack(void) {
	popGameMenu();
}

void changeBallEdit(void* self) {
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
		if (wasKeyPressed(KEY_ENTER))
		{
			clickButtonContinue();
		}

		if (gCurMenu == &gMenuMain2 && wasKeyPressed(KEY_ESC))
		{
			clickButtonContinue();
		}

		if (wasKeyPressed('h'))
		{
			clickButtonHelp();
		}

		if (wasKeyPressed('q'))
		{
			clickButtonQuit();
		}
	}
	else if (gCurMenu == &gMenuNext)
	{
		if (wasKeyPressed(KEY_ENTER))
		{
			clickButtonContinue();
		}

		if (wasKeyPressed(KEY_ESC))
		{
			clickButtonBack();
		}
	}
	else if (gCurMenu == &gMenuHelp)
	{
		if (wasKeyPressed(KEY_ESC))
		{
			clickButtonBack();
		}
	}
	else if (gCurMenu == &gMenuEnd)
	{
		if (wasKeyPressed(KEY_ENTER))
		{
			clickButtonAgain();
		}
		if (wasKeyPressed(KEY_ESC) || wasKeyPressed('q'))
		{
			clickButtonQuit();
		}
	}
	else if (gCurMenu == &gMenuWait)
	{
		if (wasKeyPressed(KEY_ESC) || wasKeyPressed('q'))
		{
			clickButtonQuit();
		}

		if (sgIdleProgress >= 1.0f)
		{
			resetBall();
			popGameMenu();
		}
	}
}

void setGameMenuPosistion(Vector3 pos) {
	gGameMenuPosition = pos;
}

void drawGameMenu(void) {
	glEnable(GL_LIGHTING);
	glEnable(GL_COLOR_MATERIAL);

		glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);

		glColor3f(1.0f, 1.0f, 1.0f);

		setSomeLight();

		glPushMatrix();
			glTranslatef(gGameMenuPosition.x, gGameMenuPosition.y, gGameMenuPosition.z);
			glRotatef(90.0f, 1.0f, 0.0f, 0.0f);

			drawLogo();
			drawMenu(gCurMenu);
		glPopMatrix();

	glDisable(GL_LIGHTING);
	glDisable(GL_COLOR_MATERIAL);
}

void eventGameMenu(const Vector3* position, const Vector3* direction, MouseEvent event) {
	Vector3 newPosition = sub(*position, gGameMenuPosition);

	if (newPosition.y < 0.0f && direction->y > 0.0f)
	{
		float t = -newPosition.y / direction->y;
		float x = newPosition.x + t * direction->x;
		float y = newPosition.z + t * direction->z;

		eventMenu(gCurMenu, x, y, event);
	}
}

void showGameMenu(int menu) {
	static Menu* menues[] = {
		&gMenuMain1,
		&gMenuMain2,
		&gMenuNext,
		&gMenuEnd,
		&gMenuHelp,
		&gMenuWait
	};

	Menu* newMenu = menues[menu];

	if (gCurMenu == &gMenuWait)
	{
		newMenu->back = gCurMenu->back;
		gCurMenu->back = newMenu;
	}
	else
	{
		newMenu->back = gCurMenu;
		gCurMenu = newMenu;
	}

	showMenu(gCurMenu);

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

	static ProgressBar pbProgress;

	static SpinEdit seBall;

	static Label lTextHelp[2 * LENGTH(gTextHelp)];

	static MenuItem* itemsMain1[] =
	{
		&bStart.item,
		&seBall.item,
		&gcShadows.item,
		&gcReflection.item,
		&bHelp.item,
		&bQuit.item
	};

	static MenuItem* itemsMain2[] =
	{
		&bResume.item,
		&seBall.item,
		&gcShadows.item,
		&gcReflection.item,
		&bHelp.item,
		&bQuit.item
	};

	static MenuItem* itemsNext[] =
	{
		&bContinue.item,
		&bMain.item
	};

	static MenuItem* itemsEnd[] =
	{
		&bAgain.item,
		&bQuit2.item
	};

	static MenuItem* itemsWait[] =
	{
		&pbProgress.item
	};

	static MenuItem* itemsHelp[LENGTH(lTextHelp) + 1];

	int i;

	initLogo();

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

	/* main menu */
	initButton(&bStart, 6.0f, clickButtonContinue, "Start");
	initButton(&bResume, 6.0f, clickButtonContinue, "Resume");

	initSpinEdit(&seBall, gCntBallLayouts - 1, 0, gCntBallLayouts - 1, 5.2f, drawMenuBall, changeBallEdit);

	initCheck(&gcShadows, 4.0f, changeShadows, "Shadows");
	initCheck(&gcReflection, 3.0f, changeReflection, "Reflection");

	initButton(&bHelp, 2.0f, clickButtonHelp, "Help");
	initButton(&bQuit, 1.0f, clickButtonQuit, "Quit");

	INIT_MENU(&gMenuMain1, itemsMain1);
	INIT_MENU(&gMenuMain2, itemsMain2);

	/* next level menu */
	initButton(&bContinue, 5.5f, clickButtonContinue, "Continue");
	initButton(&bMain, 4.5f, clickButtonBack, "Main Menu");

	INIT_MENU(&gMenuNext, itemsNext);

	/* help menu */
	for (i = 0; i < LENGTH(lTextHelp); i++)
	{
		int row = i / 2;
		int col = i % 2;
		float z = 6.0f - row;

		initLabel(&lTextHelp[i], col ? 5.0f : -5.0f, z, col, col ? gTextHelp[row].right : gTextHelp[row].left);

		itemsHelp[i] = &lTextHelp[i].item;
	}

	initButton(&bBack, 6.0f - LENGTH(gTextHelp), clickButtonBack, "back");

	itemsHelp[LENGTH(lTextHelp)] = &bBack.item;

	INIT_MENU(&gMenuHelp, itemsHelp);

	/* game complete menu */
	initButton(&bAgain, 5.5f, clickButtonAgain, "Play Again");
	initButton(&bQuit2, 4.5f, clickButtonQuit, "Quit");

	INIT_MENU(&gMenuEnd, itemsEnd);

	/* wait menu */
	initProgressBar(&pbProgress, 5.0f, &sgIdleProgress);

	INIT_MENU(&gMenuWait, itemsWait);
}
