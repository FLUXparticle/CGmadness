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
#include "environment.h"
#include "editor.h"
#include "common.h"

#include "menumanager.h"
#include "gui.h"

#include "keyboard.h"

#include <GL/glut.h>

#include <stdlib.h>

static Screen gScreenMain;
static Screen gScreenChoose;

static int gLoadedLevel = -1;

static void clickButtonCGMadness(void)
{
	pushScreen(&gScreenChoose);
}

static void clickButtonCGMEditor(void)
{
	/* TODO */
}

static void clickButtonQuit(void)
{
	exit(0);
}

static void clickButtonChoose(void)
{
	popAllScreens();
	setMainState(STATE_GAME);
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
	
	if (gLoadedLevel >= 0)
	{
		destroyLevel();
		gLoadedLevel = -1;
	}
	
	if (loadLevelFromFile(sgLevels.strings[spinedit->value], 1))
	{
		updateTexCoords();
		gLoadedLevel = spinedit->value;
	}
}

void initMainMenu(void)
{
	static Button bCGMadness;
	static Button bCGMEditor;
	static Button bQuit;
	static Button bChoose;
	static Button bBack;
	
	static SpinEdit seLevel;

	static MenuItem* itemsMain[] =
	{
		&bCGMadness.item,
		&bCGMEditor.item,
		&bQuit.item
	};
	
	static MenuItem* itemsChoose[] =
	{
		&seLevel.item,
		&bChoose.item,
		&bBack.item
	};
	
	initButton(&bCGMadness, 6.0f, clickButtonCGMadness, "CG Madness", KEY_ENTER);
	initButton(&bCGMEditor, 4.0f, clickButtonCGMEditor, "CGM Editor", 0);
	initButton(&bQuit,      2.0f, clickButtonQuit, "Quit", 'q');
	
	INIT_SCREEN(&gScreenMain, itemsMain);

	initSpinEdit(&seLevel, 0, 0, sgLevels.count - 1, 5.2f, drawMenuLevel, changeLevelChooser);

	initButton(&bChoose,    3.0f, clickButtonChoose, "choose", KEY_ENTER);
	initButton(&bBack,      2.0f, clickButtonBack, "back", KEY_ESC);
	
	INIT_SCREEN(&gScreenChoose, itemsChoose);
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
	if (getCurScreen() == &gScreenChoose)
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
