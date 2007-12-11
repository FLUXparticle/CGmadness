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

#include "mainmenu.h"

#include "main.h"
#include "field.h"
#include "level.h"
#include "texture.h"
#include "environment.h"
#include "editor.h"
#include "common.h"
#include "highscore.h"

#include "menumanager.h"
#include "gui.h"

#include "keyboard.h"

#include <GL/glut.h>

#include <stdlib.h>
#include <string.h>

static Screen gScreenMain;
static Screen gScreenChooseGame;
static Screen gScreenChooseEditor;

static int gLoadedLevel = -1;

static void clickButtonCGMadness(void)
{
	pushScreen(&gScreenChooseGame);
}

static void clickButtonCGMEditor(void)
{
	pushScreen(&gScreenChooseEditor);
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
	popScreen();
}

static void drawMenuLevel(void)
{
	/* empty */
}

static void changeLevelChooser(const void* self)
{
	const SpinEdit* spinedit = self;
	
	if (spinedit->value != gLoadedLevel)
	{
		if (gLoadedLevel >= 0)
		{
			destroyLevel();
			gLoadedLevel = -1;
		}
		
		if (loadLevelFromFile(sgLevels.strings[spinedit->value], 1))
		{
			updateTexCoords();
			gLoadedLevel = spinedit->value;
			
			if (sgLevel.borderTexture == 0) {
#if (NOISE_TEXTURE)
				sgLevel.borderTexture = loadTexture("data/boarder.tga", 1);
#else
				sgLevel.borderTexture = loadTexture("data/plate.tga", 1);
#endif
			}

			sgCurLevelname = sgLevels.strings[gLoadedLevel] + strlen(sgLevels.strings[gLoadedLevel]) + 1;
		}
	}
	
	sgLastPlayerIndex = MAX_SCORE_COLS;
}

void initMainMenu(void)
{
	static Button bCGMadness;
	static Button bCGMEditor;
	static Button bQuit;
	static Button bChooseGame;
	static Button bChooseEditor;
	static Button bBack;
	
	static SpinEdit seLevel;

	static HighScore hsHighScore;

	static MenuItem* itemsMain[] =
	{
		&bCGMadness.item,
		&bCGMEditor.item,
		&bQuit.item
	};
	
	static MenuItem* itemsChooseGame[] =
	{
		&seLevel.item,
		&hsHighScore.item,
		&bChooseGame.item,
		&bBack.item
	};
	
	static MenuItem* itemsChooseEditor[] =
	{
		&seLevel.item,
		&bChooseEditor.item,
		&bBack.item
	};
	
	initButton(&bCGMadness, 6.0f, clickButtonCGMadness, "CG Madness", KEY_ENTER);
	initButton(&bCGMEditor, 4.0f, clickButtonCGMEditor, "CGM Editor", 0);
	initButton(&bQuit,      2.0f, clickButtonQuit, "Quit", 'q');
	
	INIT_SCREEN(&gScreenMain, itemsMain);

	initSpinEdit(&seLevel, 0, 0, sgLevels.count - 1, 7.0, 5.0f, drawMenuLevel, changeLevelChooser);

	initHighScore(&hsHighScore, 3.0f);

	initButton(&bChooseGame,    2.0f, clickButtonChooseGame, "choose", KEY_ENTER);
	initButton(&bBack,      1.0f, clickButtonBack, "back", KEY_ESC);
	
	INIT_SCREEN(&gScreenChooseGame, itemsChooseGame);
	
	initButton(&bChooseEditor,    2.0f, clickButtonChooseEditor, "choose", KEY_ENTER);
	
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

void drawMainMenu(void)
{
	if (getCurScreen() == &gScreenChooseGame)
	{
		drawEnvironment(drawEditorField);
		drawEditorField();
	}
	else if (getCurScreen() == &gScreenChooseEditor)
	{
		drawEnvironment(drawEditorField);
		drawEditorField();
	}
	else
	{
		drawEnvironment(NULL);
	}
	
	drawMenuManager();
}
