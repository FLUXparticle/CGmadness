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
#include "text.h"
#include "level.h"
#include "game.h"
#include "features.h"
#include "keyboard.h"
#include "gui.h"
#include "menumanager.h"
#include "debug.h"
#include "main.h"
#include "objects.h"

#include <stdio.h>
#include <string.h>

#define HIGHSCORE_WIDTH 4.0f
#define HIGHSCORE_HEIGHT 4.0f

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

static Screen gScreenMain1;
static Screen gScreenMain2;
static Screen gScreenHelp;
static Screen gScreenNext;
static Screen gScreenEnd;

int sgLastPlayerIndex;

/* events */

static int gCntBallLayouts;
static int gBallLayouts[MAX_BALL_LAYOUTS];

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

static void clickButtonStart(void)
{
	popAllScreens();
	resetBall();
	resumeGame();
}

static void clickButtonResume(void)
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

static void changeShadows(void* self) {
	Check* check = self;
	setShadows(check->value);
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

	setCheck(&gcShadows, useShadows());
	setCheck(&gcReflection, useReflection());
}

void updateHighScore(float interval)
{
	unsigned char ch = getLastChar();
	
	if (sgLastPlayerIndex >= 0 && wasKeyPressed(ch))
	{
		char* name = sgLevel.scores[sgLastPlayerIndex].name;
		int len = strlen(name);

		switch (ch)
		{
		case '\b':
			name[len - 1] = '\0';
			break;
		case KEY_ENTER:
			saveHighscoreToFile();
			sgLastPlayerIndex = -1;
			break;
		default:
			if (ch >= MIN_ALLOWED_CHAR && ch <= MAX_ALLOWED_CHAR)
			{
				if (len < MAX_NAME_LENGTH)
				{
					name[len] = ch;
					name[len + 1] = '\0';
				}
			}
		}
	}
}

void drawHighScore(void)
{
	int i;
	
	float scale = 0.5f * HIGHSCORE_HEIGHT / (MAX_SCORE_COLS + 1);
	
	glPushMatrix();
	
		glTranslatef(HIGHSCORE_WIDTH / 2.0f, HIGHSCORE_HEIGHT / 2.0f, -0.1f);
		
		glScalef(HIGHSCORE_WIDTH / 2.0f, HIGHSCORE_HEIGHT / 2.0f, 1.0f);
		
		glColor4f(0.0f, 0.0f, 0.0f, 0.5);
		
		glEnable(GL_BLEND);

			drawSquare();
		
		glDisable(GL_BLEND);
	
	glPopMatrix();

	for (i = 0; i < sgLevel.cntScoreCols; i++)
	{
		char strName[MAX_NAME_LENGTH + 4];
		char strTime[10];
		
		int tenthSecond = sgLevel.scores[i].tenthSecond;
		
		sprintf(strName, "%2d%c%s", i + 1, 0, sgLevel.scores[i].name);
		sprintf(strTime, "%d:%02d.%01d", tenthSecond / 600, tenthSecond / 10 % 60, tenthSecond % 10);
		
		if (i == sgLastPlayerIndex)
		{
			glColor3f(1.0f, 0.0f, 0.0f);
		}
		else
		{
			glColor3f(1.0f, 1.0f, 1.0f);
		}
		
		glPushMatrix();
		
			glTranslatef(0.0f, (float) (MAX_SCORE_COLS - i) / (MAX_SCORE_COLS + 1) * HIGHSCORE_HEIGHT, 0.0f);
		
			glScalef(scale, scale, scale);

			glPushMatrix();
			
				glTranslatef((0.1f * HIGHSCORE_WIDTH / scale) - widthStrokeText(strName), 0.0f, 0.0f);
				
				strName[2] = ' ';
				
				drawStrokeThinText(strName);
		
			glPopMatrix();
				
			glTranslatef((0.95f * HIGHSCORE_WIDTH / scale) - widthStrokeText(strTime), 0.0f, 0.0f);
			
			drawStrokeThinText(strTime);
		
		glPopMatrix();
	}
	
	glColor3f(1.0f, 1.0f, 1.0f);
}

void initGameMenu(void) {
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

	static Canvas cHighScore;

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
		&cHighScore.item,
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
	initButton(&bStart, 6.0f, clickButtonStart, "Start", KEY_ENTER);
	initButton(&bResume, 6.0f, clickButtonResume, "Resume", KEY_ENTER);

	initSpinEdit(&seBall, gCntBallLayouts - 1, 0, gCntBallLayouts - 1, 5.2f, drawMenuBall, changeBallEdit);

	initCheck(&gcShadows, 4.0f, changeShadows, "Shadows");
	initCheck(&gcReflection, 3.0f, changeReflection, "Reflection");

	initButton(&bHelp, 2.0f, clickButtonHelp, "Help", 'h');
	initButton(&bQuit, 1.0f, clickButtonQuit, "Quit", 'q');

	INIT_SCREEN(&gScreenMain1, itemsMain1);
	INIT_SCREEN(&gScreenMain2, itemsMain2);

	/* next level menu */
	initButton(&bContinue, 5.5f, clickButtonResume, "Continue", KEY_ENTER);
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
	
	initCanvas(&cHighScore, 3.0f, HIGHSCORE_WIDTH, HIGHSCORE_HEIGHT, updateHighScore, drawHighScore);
	
	initButton(&bAgain, 2.0f, clickButtonAgain, "Play Again", KEY_ENTER);
	initButton(&bQuit2, 1.0f, clickButtonQuit, "Quit", 'q');

	INIT_SCREEN(&gScreenEnd, itemsEnd);
}
