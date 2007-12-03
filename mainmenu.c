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

#include "menumanager.h"
#include "gui.h"

#include "keyboard.h"

#include <GL/glut.h>

#include <stdlib.h>

static Screen gScreenMain;
static Screen gScreenChoose;

static void clickButtonCGMadness(void)
{
	popAllScreens();
	setMainState(STATE_GAME);
}

static void clickButtonCGMEditor(void)
{
	loadFieldFromFile(sgLevels.strings[0]);
	initGameField();
	updateGameField();
	pushScreen(&gScreenChoose);
}

static void clickButtonQuit(void)
{
	exit(0);
}

static void clickButtonBack(void)
{
	popScreen();
}

static void drawMenuLevel(void)
{
	glPushMatrix();
	
		glScalef(0.1f, 0.1f, 0.1f);
		
		glRotatef(-90.0f, 1.0f, 0.0f, 0.0f);
		
		drawGameField(0);
		
	glPopMatrix();
}

static void changeLevelChooser(void* self)
{
}

void initMainMenu(void)
{
	static Button bCGMadness;
	static Button bCGMEditor;
	static Button bQuit;
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
		&bBack.item
	};
	
	initButton(&bCGMadness, 6.0f, clickButtonCGMadness, "CG Madness", KEY_ENTER);
	initButton(&bCGMEditor, 4.0f, clickButtonCGMEditor, "CGM Editor", 0);
	initButton(&bQuit,      2.0f, clickButtonQuit, "Quit", 'q');
	
	INIT_SCREEN(&gScreenMain, itemsMain);

	initSpinEdit(&seLevel, 0, 0, sgLevels.count - 1, 5.2f, drawMenuLevel, changeLevelChooser);

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
	drawEnvironment(NULL);
	drawMenuManager();
}
