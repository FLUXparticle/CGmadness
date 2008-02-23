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

#include "gamemenu.hpp"

#include "gui/Label.hpp"
#include "gui/Button.hpp"
#include "gui/Check.hpp"
#include "gui/SpinEdit.hpp"

#include "PlayersBall.hpp"
#include "text.hpp"
#include "level.hpp"
#include "Game.hpp"
#include "features.hpp"
#include "keyboard.hpp"
#include "highscore.hpp"
#include "main.hpp"
#include "objects.hpp"

#include "menumanager.hpp"
#include "gui.hpp"

#include <stdio.h>
#include <string.h>

typedef struct
{
	char *left;
	char *right;
} LeftRight;

static Check gcBallShadow;
static Check gcReflection;

static SpinEdit gseBall;

/*
 * help text
 */
static LeftRight gTextHelp[] = {
	{"Cursor", "Move"},
	{"Space", "Jump"},
	{"W A S D", "Camera"},
	{"R F", "Zoom"},
	{"Enter", "Reset"},
	{"Esc", "Menu"},
};

static Screen gScreenMain1;
static Screen gScreenMain2;
static Screen gScreenHelp;
static Screen gScreenEnd;

/* events */

static int gCntBallLayouts;
static int gBallLayouts[MAX_BALL_LAYOUTS];

static void clickButtonHelp(void)
{
	pushScreen(&gScreenHelp);
}

static void clickButtonQuit(void)
{
	popScreen();
	setMainState(STATE_MAIN);
}

static void clickButtonQuit2(void)
{
	acceptHighScoreName();
	clickButtonQuit();
}

static void clickButtonAgain(void)
{
	acceptHighScoreName();
	popScreen();
	resetGame();
}

static void clickButtonStart(void)
{
	popScreen();
	resumeGame();
}

static void clickButtonResume(void)
{
	popScreen();
	resumeGame();
}

static void clickButtonBack(void)
{
	popScreen();
}

static void changeBallEdit(const void *self)
{
	PlayersBall::sgoBall.changeBall(gBallLayouts[((SpinEdit *) self)->value]);
}

static void changeBallShadow(const void *self)
{
	const Check *check = (const Check *) self;
	setBallShadow(check->value);
}

static void changeReflection(const void *self)
{
	const Check *check = (const Check *) self;
	setReflection(check->value);
}

void showGameMenu(int menu)
{
	static Screen *screens[] = {
		&gScreenMain1,
		&gScreenMain2,
		&gScreenEnd
	};

	pushScreen(screens[menu]);

	gcBallShadow.set(useBallShadow());
	gcReflection.set(useReflection());
	changeBallEdit(&gseBall);
}

void initGameMenu(void)
{
	static Button bStart;
	static Button bResume;
	static Button bQuit;
	static Button bQuit2;
	static Button bHelp;
	static Button bBack;
	static Button bAgain;

	static HighScore hsHighScore;

	static Label lTextHelp[2 * LENGTH(gTextHelp)];

	static MenuItem *itemsMain1[] = {
		&bStart,
		&gseBall,
		&gcBallShadow,
		&gcReflection,
		&bHelp,
		&bQuit
	};

	static MenuItem *itemsMain2[] = {
		&bResume,
		&gseBall,
		&gcBallShadow,
		&gcReflection,
		&bHelp,
		&bQuit
	};

	static MenuItem *itemsEnd[] = {
		&hsHighScore.item,
		&bAgain,
		&bQuit2
	};

	static MenuItem *itemsHelp[LENGTH(lTextHelp) + 1];

	int i;

	/*
	 * which ball layouts are available?
	 */
	gCntBallLayouts = 0;

	gBallLayouts[gCntBallLayouts++] = BALL_LAYOUT_DEFAULT;

	if (Ball::hasBallTexture())
	{
		gBallLayouts[gCntBallLayouts++] = BALL_LAYOUT_TEXTURE;
	}

	if (PlayersBall::sgoBall.hasCubeMap())
	{
		gBallLayouts[gCntBallLayouts++] = BALL_LAYOUT_METAL;
	}

	if (hasGolfballShader())
	{
		gBallLayouts[gCntBallLayouts++] = BALL_LAYOUT_GOLFBALL;
	}

	if (hasGolfballShader() && PlayersBall::sgoBall.hasCubeMap())
	{
		gBallLayouts[gCntBallLayouts++] = BALL_LAYOUT_GOLFBALL_METAL;
	}

	/*
	 * put all together
	 */

	/* main menu */
	bStart = Button(6.0f, clickButtonStart, "start", KEY_ENTER);
	bResume = Button(6.0f, clickButtonResume, "resume", KEY_ENTER);

	gseBall = SpinEdit(gCntBallLayouts - 1, 0, gCntBallLayouts - 1, 4.3, 5.2f, drawMenuBall, changeBallEdit);

	gcBallShadow = Check(4.0f, changeBallShadow, "ball shadow");
	gcReflection = Check(3.0f, changeReflection, "reflection");

	bHelp = Button(2.0f, clickButtonHelp, "help", 'h');
	bQuit = Button(1.0f, clickButtonQuit, "give up", KEY_ESC);

	INIT_SCREEN(&gScreenMain1, itemsMain1);
	INIT_SCREEN(&gScreenMain2, itemsMain2);

	/* help menu */
	for (i = 0; i < LENGTH(lTextHelp); i++)
	{
		int row = i / 2;
		int col = i % 2;
		float z = 6.0f - row;

		lTextHelp[i] = Label(col ? 5.0f : -5.0f, z, 1.0f, col,
							col ? gTextHelp[row].right : gTextHelp[row].left);

		itemsHelp[i] = &lTextHelp[i];
	}

	bBack = Button(6.0f - LENGTH(gTextHelp), clickButtonBack, "back", KEY_ESC);

	itemsHelp[LENGTH(lTextHelp)] = &bBack;

	INIT_SCREEN(&gScreenHelp, itemsHelp);

	/* game complete menu */

	initHighScore(&hsHighScore, 3.0f);

	bAgain = Button(2.0f, clickButtonAgain, "play again", KEY_ENTER);
	bQuit2 = Button(1.0f, clickButtonQuit2, "change level", KEY_ESC);

	INIT_SCREEN(&gScreenEnd, itemsEnd);
}
