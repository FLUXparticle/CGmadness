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
static Check gcFog;

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

static Object goHelpText;
static Object goMainText;

static int gIsPauseMenu;

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

static void changeFog(void* self) {
	Check* check = self;
	setFog(check->value);
}

static void clickButtonHelp(void) {
	goHelpText.visible = 1;
	goMainText.visible = 0;
}

static void clickButtonQuit(void) {
	exit(0);
}

static void clickButtonBack(void) {
	goHelpText.visible = 0;
	goMainText.visible = 1;
}

void updateGameMenu(float interval) {
	if (goMainText.visible) {
		if (!gIsPauseMenu && wasKeyPressed(KEY_ENTER)) {
			clickButtonStart();
		}
		
		if (gIsPauseMenu && wasKeyPressed(KEY_ESC)) {
			clickButtonStart();
		}

		if (wasKeyPressed('h')) {
			clickButtonHelp();
		}

		if (wasKeyPressed('q')) {
			clickButtonQuit();
		}
	} else if (goHelpText.visible) {
		if (wasKeyPressed(KEY_ESC)) {
			clickButtonBack();
		}
	}
}

void showGameMenu(int pause) {
	gbStart.oButton.visible = !pause;
	gbResume.oButton.visible = pause;

	setCheck(&gcFog, useFog());
	setCheck(&gcShadows, useShadows());

	gIsPauseMenu = pause;

	glutSetCursor(GLUT_CURSOR_LEFT_ARROW);
}

void initGameMenu(Object* obj) {
	static Button bQuit;
	static Button bHelp;
	static Button bBack;
	
	static SpinEdit spinEditBall;

	static Object oLogo;	
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

	initObjectGroup(obj);
	
	/* menu logo */
	initObject(&oLogo, drawSquare);
	oLogo.texture = loadTexture("data/logo.tga", 0);
	setObjectPosition3f(&oLogo, 0.0f, 0.0f, 8.0f);
	setObjectScale3f(&oLogo, 4.0f, 1.0f, 1.0f);

	rotateObjectX(&oLogo, 90.0f);
	addSubObject(obj, &oLogo);

	/* main menu */
	initObjectGroup(&goMainText);

	initObject(&oBall, drawMenuBall);

	init3dButton(&gbStart, 6.0f, clickButtonStart, "Start");
  addSubObject(&goMainText, &gbStart.oButton);
	
	init3dButton(&gbResume, 6.0f, clickButtonStart, "Resume");
	gbResume.oButton.visible = 0;
  addSubObject(&goMainText, &gbResume.oButton);

	gIsPauseMenu = 0;
	
	init3dSpinEdit(&spinEditBall, gCntBallLayouts - 1, 0, gCntBallLayouts - 1, 5.2f, &oBall, changeBallEdit);
	addSubObject(&goMainText, &spinEditBall.oSpinEdit);
	
	init3dCheck(&gcShadows, 4.0f, changeShadows, "Shadows");
  addSubObject(&goMainText, &gcShadows.oCheck);

	init3dCheck(&gcFog, 3.0f, changeFog, "Fog");
  addSubObject(&goMainText, &gcFog.oCheck);

	init3dButton(&bHelp, 2.0f,clickButtonHelp, "Help");
  addSubObject(&goMainText, &bHelp.oButton);
	
	init3dButton(&bQuit, 1.0f, clickButtonQuit, "Quit");
  addSubObject(&goMainText, &bQuit.oButton);

	addSubObject(obj, &goMainText);

	/* help text */
	initObjectGroup(&goHelpText);
	
	goHelpText.visible = 0;

	for (i = 0; i < LENGTH(gTextHelp); i++) {
		float z = 6.0f - i;
		float length;

		{
			Object* o = &oTextHelp[2 * i];

			length = makeTextObject(o, gTextHelp[i].left) * SCALE_FONT;
			setObjectPosition3f(o, -5.0f, 0.0f, z);
			setObjectScalef(o, SCALE_FONT);
			rotateObjectX(o, 90.0f);
			
	  	addSubObject(&goHelpText, o);
		}

		{
			Object* o = &oTextHelp[2 * i + 1];
			
			length = makeTextObject(o, gTextHelp[i].right) * SCALE_FONT;
			setObjectPosition3f(o, 5.0f - length, 0.0f, z);
			setObjectScalef(o, SCALE_FONT);
			rotateObjectX(o, 90.0f);
			
	  	addSubObject(&goHelpText, o);
		}
	}

	init3dButton(&bBack, 6.0f - LENGTH(gTextHelp), clickButtonBack, "back");
 	addSubObject(&goHelpText, &bBack.oButton);

	addSubObject(obj, &goHelpText);
}
