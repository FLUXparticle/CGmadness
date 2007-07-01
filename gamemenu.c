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
#include "pick.h"
#include "objects.h"
#include "texture.h"
#include "ball.h"
#include "game.h"
#include "features.h"
#include "keyboard.h"
#include "gui.h"

#include <GL/glut.h>

#include <string.h>

#define SCALE_FONT 0.5f

typedef struct {
	char* left;
	char* right;
} LeftRight;

static Button gbStart;
static Button gbResume;

static Check gcShadows;

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

static Object goLogo;
static Object goMainMenu;
static Object goNextMenu;
static Object goHelpMenu;
static Object goEndMenu;

static Object* gCurMenu;

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

static void clickButtonHelp(void) {
	gCurMenu = &goHelpMenu;
}

static void clickButtonQuit(void) {
	exit(0);
}

static void clickButtonAgain(void) {
	resetGame();
}

static void clickButtonBack(void) {
	gCurMenu = &goMainMenu;
}

void updateGameMenu(float interval) {
	if (gCurMenu == &goMainMenu) {
		if (wasKeyPressed(KEY_ENTER)) {
			clickButtonStart();
		}

		if (gbResume.oButton.visible && wasKeyPressed(KEY_ESC)) {
			clickButtonStart();
		}

		if (wasKeyPressed('h')) {
			clickButtonHelp();
		}

		if (wasKeyPressed('q')) {
			clickButtonQuit();
		}
	} else if (gCurMenu == &goNextMenu) {
		if (wasKeyPressed(KEY_ENTER)) {
			clickButtonStart();
		}

		if (wasKeyPressed(KEY_ESC)) {
			clickButtonBack();
		}
	} else if (gCurMenu == &goHelpMenu) {
		if (wasKeyPressed(KEY_ESC)) {
			clickButtonBack();
		}
	} else if (gCurMenu == &goEndMenu) {
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

			drawObject(&goLogo);
			drawObject(gCurMenu);

		glPopMatrix();
	glDisable(GL_LIGHTING);
}

void pickGameMenu(void) {
	glPushMatrix();
		glTranslatef(gGameMenuPosition.x, gGameMenuPosition.y, gGameMenuPosition.z);

		pickObject(gCurMenu);

	glPopMatrix();
}

void showGameMenu(int menu) {
	gbStart.oButton.visible = menu == 0;
	gbResume.oButton.visible = menu != 0;

	switch (menu) {
	case 0:
	case 1:
		gCurMenu = &goMainMenu;
		break;
	case 2:
		gCurMenu = &goNextMenu;
		break;
	case 3:
		gCurMenu = &goEndMenu;
		break;
	}

	setCheck(&gcShadows, useShadows());

	glutSetCursor(GLUT_CURSOR_LEFT_ARROW);
}

void initGameMenu() {
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
	setObjectPosition3f(&goLogo, 0.0f, 0.0f, 8.0f);
	setObjectScale3f(&goLogo, 4.0f, 1.0f, 1.0f);

	rotateObjectX(&goLogo, 90.0f);

	/* main menu */
	initObjectGroup(&goMainMenu);

	initObject(&oBall, drawMenuBall);

	init3dButton(&gbStart, 5.5f, clickButtonStart, "Start");
  addSubObject(&goMainMenu, &gbStart.oButton);

	init3dButton(&gbResume, 5.5f, clickButtonStart, "Resume");
  addSubObject(&goMainMenu, &gbResume.oButton);

	init3dSpinEdit(&spinEditBall, gCntBallLayouts - 1, 0, gCntBallLayouts - 1, 4.7f, &oBall, changeBallEdit);
	addSubObject(&goMainMenu, &spinEditBall.oSpinEdit);

	init3dCheck(&gcShadows, 3.5f, changeShadows, "Shadows");
  addSubObject(&goMainMenu, &gcShadows.oCheck);

	init3dButton(&bHelp, 2.5f,clickButtonHelp, "Help");
  addSubObject(&goMainMenu, &bHelp.oButton);

	init3dButton(&bQuit, 1.5f, clickButtonQuit, "Quit");
  addSubObject(&goMainMenu, &bQuit.oButton);

	/* next level menu */
	initObjectGroup(&goNextMenu);

	init3dButton(&bContinue, 5.5f, clickButtonStart, "Continue");
  addSubObject(&goNextMenu, &bContinue.oButton);

	init3dButton(&bMain, 4.5f, clickButtonBack, "Main Menu");
 	addSubObject(&goNextMenu, &bMain.oButton);

	/* help menu */
	initObjectGroup(&goHelpMenu);

	for (i = 0; i < LENGTH(gTextHelp); i++) {
		float z = 6.0f - i;
		float length;

		{
			Object* o = &oTextHelp[2 * i];

			length = makeTextObject(o, gTextHelp[i].left) * SCALE_FONT;
			setObjectPosition3f(o, -5.0f, 0.0f, z);
			setObjectScalef(o, SCALE_FONT);
			rotateObjectX(o, 90.0f);

	  	addSubObject(&goHelpMenu, o);
		}

		{
			Object* o = &oTextHelp[2 * i + 1];

			length = makeTextObject(o, gTextHelp[i].right) * SCALE_FONT;
			setObjectPosition3f(o, 5.0f - length, 0.0f, z);
			setObjectScalef(o, SCALE_FONT);
			rotateObjectX(o, 90.0f);

	  	addSubObject(&goHelpMenu, o);
		}
	}

	init3dButton(&bBack, 6.0f - LENGTH(gTextHelp), clickButtonBack, "back");
 	addSubObject(&goHelpMenu, &bBack.oButton);

	/* game complete menu */
	initObjectGroup(&goEndMenu);

	init3dButton(&bAgain, 5.5f, clickButtonAgain, "Play Again");
  addSubObject(&goEndMenu, &bAgain.oButton);

	init3dButton(&bQuit2, 4.5f, clickButtonQuit, "Quit");
 	addSubObject(&goEndMenu, &bQuit2.oButton);

}
