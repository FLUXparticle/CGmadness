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
 */

#include "mainmenu.hpp"

#include "gui/Button.hpp"
#include "gui/SpinEdit.hpp"

#include "main.hpp"
#include "field.hpp"
#include "level.hpp"
#include "texture.hpp"
#include "environment.hpp"
#include "Editor.hpp"
#include "common.hpp"
#include "highscore.hpp"

#include "menumanager.hpp"
#include "gui.hpp"
#include "text.hpp"

#include "keyboard.hpp"

#include <GL/glut.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define LEVELINFO_WIDTH 4.0f
#define LEVELINFO_HEIGHT 4.0f
#define LEVELINFO_LINES 11

static Screen gScreenMain;
static Screen gScreenChooseGame;
static Screen gScreenChooseEditor;

static SpinEdit gseLevel;

static int gLoadedLevel = -1;

static void loadLevel(int index)
{
	if (index != gLoadedLevel)
	{
		if (gLoadedLevel >= 0)
		{
			destroyLevel();
			gLoadedLevel = -1;
		}

		if (index >= 0 && loadLevelFromFile(sgLevels.strings[index], 1))
		{
			updateTexCoords();
			gLoadedLevel = index;

			if (sgLevel.borderTexture == 0)
			{
				sgLevel.borderTexture = loadTexture("data/plate.tga", true);
			}

			sgCurLevelname =
				sgLevels.strings[gLoadedLevel] +
				strlen(sgLevels.strings[gLoadedLevel]) + 1;
		}
	}
}

static void changeLevelChooser(const void *self)
{
	const SpinEdit *spinedit = (const SpinEdit *) self;

	if (getCurScreen() == &gScreenChooseGame
			|| getCurScreen() == &gScreenChooseEditor)
	{
		loadLevel(spinedit->value);
	}

	sgLastPlayerIndex = MAX_SCORE_COLS;
}

static void clickButtonCGMadness(void)
{
	pushScreen(&gScreenChooseGame);
	changeLevelChooser(&gseLevel);
}

static void clickButtonCGMEditor(void)
{
	pushScreen(&gScreenChooseEditor);
	changeLevelChooser(&gseLevel);
}

static void clickButtonQuit(void)
{
	exit(0);
}

static void clickButtonChooseGame(void)
{
	setMainState(STATE_GAME);
}

static void clickButtonChooseEditor(void)
{
	setMainState(STATE_EDITOR);
}

static void clickButtonBack(void)
{
	loadLevel(-1);
	popScreen();
}

static void drawMenuLevel(void)
{
	/* empty */
}

static void updateLevelInfo(float interval)
{
	/* empty */
}

static void drawLevelInfo(void)
{
	const char *lines[LEVELINFO_LINES];
	char size[20];
	int i;

	float scale = 0.5f * LEVELINFO_HEIGHT / (LEVELINFO_LINES + 1);

	sprintf(size, "size: %d x %d", sgLevel.size.x, sgLevel.size.y);

	lines[0] = sgCurLevelname;
	lines[1] = "";
	lines[2] = "";
	lines[3] = "";
	lines[4] = "";
	lines[5] = size;
	lines[6] = "";
	lines[7] = "";
	lines[8] = "";
	lines[9] = "";
	lines[10] = "";

	drawPanel(LEVELINFO_WIDTH, LEVELINFO_HEIGHT);

	for (i = 0; i < LEVELINFO_LINES; i++)
	{
		glPushMatrix();

		glTranslatef(LEVELINFO_WIDTH / 2.0f,
								 (float) (LEVELINFO_LINES - i) / (LEVELINFO_LINES +
																									1) * LEVELINFO_HEIGHT, 0.0f);

		glScalef(scale, scale, scale);

		glTranslatef(-widthStrokeText(lines[i]) / 2.0f, 0.0f, 0.0f);

		switch (i)
		{
		case 0:
			glColor3f(0.0f, 0.0f, 1.0f);
			break;
		case 5:
			glColor3f(1.0f, 1.0f, 0.0f);
			break;
		default:
			glColor3f(1.0f, 1.0f, 1.0f);
			break;
		}

		drawStrokeThinText(lines[i]);

		glPopMatrix();
	}

	glColor3f(1.0f, 1.0f, 1.0f);
}

void initMainMenu(void)
{
	static Button bCGMadness;
	static Button bCGMEditor;
	static Button bQuit;
	static Button bChooseGame;
	static Button bChooseEditor;
	static Button bBack;

	static Canvas cLevelInfo;

	static HighScore hsHighScore;

	static MenuItem *itemsMain[] = {
		&bCGMadness,
		&bCGMEditor,
		&bQuit
	};

	static MenuItem *itemsChooseGame[] = {
		&gseLevel,
		&hsHighScore,
		&bChooseGame,
		&bBack
	};

	static MenuItem *itemsChooseEditor[] = {
		&gseLevel,
		&cLevelInfo,
		&bChooseEditor,
		&bBack
	};

	bCGMadness = Button(6.0f, clickButtonCGMadness, "CG Madness", KEY_ENTER);
	bCGMEditor = Button(4.0f, clickButtonCGMEditor, "CGM Editor", 'e');
	bQuit = Button(2.0f, clickButtonQuit, "Quit", 'q');

	INIT_SCREEN(&gScreenMain, itemsMain);

	gseLevel = SpinEdit(0, 0, sgLevels.count - 1, 7.0, 5.0f, drawMenuLevel, changeLevelChooser);

	initHighScore(&hsHighScore, 3.0f);

	bChooseGame = Button(2.0f, clickButtonChooseGame, "choose", KEY_ENTER);
	bBack = Button(1.0f, clickButtonBack, "back", KEY_ESC);

	INIT_SCREEN(&gScreenChooseGame, itemsChooseGame);

	cLevelInfo = Canvas(3.0f, LEVELINFO_WIDTH, LEVELINFO_HEIGHT, updateLevelInfo, drawLevelInfo);

	bChooseEditor = Button(2.0f, clickButtonChooseEditor, "choose", KEY_ENTER);

	INIT_SCREEN(&gScreenChooseEditor, itemsChooseEditor);
}

void showMainMenu(void)
{
	pushScreen(&gScreenMain);
}

void updateMainMenu(float interval)
{
	updateEnvironment(interval);
	updateMenuManager(interval);
}

void drawMainMenu(const Refectable* reflection)
{
	if (getCurScreen() == &gScreenChooseGame
			|| getCurScreen() == &gScreenChooseEditor)
	{
		drawEnvironment(reflection);
		drawEditorField();
	}
	else
	{
		drawEnvironment(NULL);
	}

	drawMenuManager();
}
