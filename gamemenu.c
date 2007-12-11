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

#include "ball.h"
#include "game.h"
#include "features.h"
#include "keyboard.h"
#include "gui.h"
#include "menumanager.h"
#include "debug.h"
#include "main.h"

typedef struct {
	char* left;
	char* right;
} LeftRight;

static Check gcBallShadow;
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

static Screen gScreenMain1;
static Screen gScreenMain2;
static Screen gScreenHelp;
static Screen gScreenNext;
static Screen gScreenEnd;

/* events */

int gCntBallLayouts;
int gBallLayouts[MAX_BALL_LAYOUTS];

static void clickButtonHelp(void)
{
	pushScreen(&gScreenHelp);
}

static void clickButtonQuit(void)
{
	setMainState(STATE_MAIN);
}

static void clickButtonAgain(void)
{
	popAllScreens();
	resetGame();
}

static void clickButtonContinue(void)
{
	popAllScreens();
	resumeGame();
}

static void clickButtonBack(void) {
	popScreen();
}

static void changeBallEdit(void* self) {
	changeBall(gBallLayouts[((SpinEdit*) self)->value]);
}

static void changeBallShadow(void* self) {
	Check* check = self;
	setBallShadow(check->value);
}

static void changeReflection(void* self) {
	Check* check = self;
	setReflection(check->value);
}

void showGameMenu(int menu) {
	static Screen* screens[] = {
		&gScreenMain1,
		&gScreenMain2,
		&gScreenNext,
		&gScreenEnd,
		&gScreenHelp
	};
	
	pushScreen(screens[menu]);

	setCheck(&gcBallShadow, useBallShadow());
	setCheck(&gcReflection, useReflection());
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

	static SpinEdit seBall;

	static Label lTextHelp[2 * LENGTH(gTextHelp)];

	static MenuItem* itemsMain1[] =
	{
		&bStart.item,
		&seBall.item,
		&gcBallShadow.item,
		&gcReflection.item,
		&bHelp.item,
		&bQuit.item
	};

	static MenuItem* itemsMain2[] =
	{
		&bResume.item,
		&seBall.item,
		&gcBallShadow.item,
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

	static MenuItem* itemsHelp[LENGTH(lTextHelp) + 1];

	int i;

	/*
	 * which ball layouts are available?
	 */
	gCntBallLayouts = 0;

	gBallLayouts[gCntBallLayouts++] = BALL_LAYOUT_DEFAULT;

	if (hasBallTexture()) {
		gBallLayouts[gCntBallLayouts++] = BALL_LAYOUT_TEXTURE;
	}

	if (hasCubeMap()) {
		gBallLayouts[gCntBallLayouts++] = BALL_LAYOUT_METAL;
	}

	if (hasGolfballShader()) {
		gBallLayouts[gCntBallLayouts++] = BALL_LAYOUT_GOLFBALL;
	}

	if (hasGolfballShader() && hasCubeMap()) {
		gBallLayouts[gCntBallLayouts++] = BALL_LAYOUT_GOLFBALL_METAL;
	}

	/*
	 * put all together
	 */

	/* main menu */
	initButton(&bStart, 6.0f, clickButtonContinue, "Start", KEY_ENTER);
	initButton(&bResume, 6.0f, clickButtonContinue, "Resume", KEY_ENTER);

	initSpinEdit(&seBall, gCntBallLayouts - 1, 0, gCntBallLayouts - 1, 5.2f, drawMenuBall, changeBallEdit);

	initCheck(&gcBallShadow, 4.0f, changeBallShadow, "ball shadow");
	initCheck(&gcReflection, 3.0f, changeReflection, "Reflection");

	initButton(&bHelp, 2.0f, clickButtonHelp, "Help", 'h');
	initButton(&bQuit, 1.0f, clickButtonQuit, "Quit", 'q');

	INIT_SCREEN(&gScreenMain1, itemsMain1);
	INIT_SCREEN(&gScreenMain2, itemsMain2);

	/* next level menu */
	initButton(&bContinue, 5.5f, clickButtonContinue, "Continue", KEY_ENTER);
	initButton(&bMain, 4.5f, clickButtonBack, "Main Menu", KEY_ESC);

	INIT_SCREEN(&gScreenNext, itemsNext);

	/* help menu */
	for (i = 0; i < LENGTH(lTextHelp); i++)
	{
		int row = i / 2;
		int col = i % 2;
		float z = 6.0f - row;

		initLabel(&lTextHelp[i], col ? 5.0f : -5.0f, z, col, col ? gTextHelp[row].right : gTextHelp[row].left);

		itemsHelp[i] = &lTextHelp[i].item;
	}

	initButton(&bBack, 6.0f - LENGTH(gTextHelp), clickButtonBack, "back", KEY_ESC);

	itemsHelp[LENGTH(lTextHelp)] = &bBack.item;

	INIT_SCREEN(&gScreenHelp, itemsHelp);

	/* game complete menu */
	initButton(&bAgain, 5.5f, clickButtonAgain, "Play Again", KEY_ENTER);
	initButton(&bQuit2, 4.5f, clickButtonQuit, "Quit", 'q');

	INIT_SCREEN(&gScreenEnd, itemsEnd);
}
