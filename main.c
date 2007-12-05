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

#include "main.h"

#include "ball.h"
#include "environment.h"

#include "mainmenu.h"
#include "game.h"
#include "editor.h"

#include "menumanager.h"

StringList sgLevels;

static MainState gCurState; 

void initMain(void)
{
	createStringListFromDir(&sgLevels, "levels");
	
	initEnvironment();

	initMenuManager();
	
	initMainMenu();
	
	setMainState(STATE_MAIN);
	
	initGame();

	initEditor();
}

void setMainState(MainState newState)
{
	gCurState = newState;
	switch (gCurState)
	{
	case STATE_MAIN:
		showMainMenu();
		break;
	case STATE_GAME:
		startGame();
		break;
	case STATE_EDITOR:
		break;
	}
}

void updateMain(float interval)
{
	switch (gCurState)
	{
	case STATE_MAIN:
		updateMainMenu(interval);
		break;
	case STATE_GAME:
		updateGame(interval);
		break;
	case STATE_EDITOR:
		updateEditor(interval);
		break;
	}
}

void drawMain(void)
{
	switch (gCurState)
	{
	case STATE_MAIN:
		drawMainMenu();
		break;
	case STATE_GAME:
		drawGame();
		break;
	case STATE_EDITOR:
		drawEditor();
		break;
	}
}

void drawMainHUD(float width, float height)
{
	switch (gCurState)
	{
	case STATE_MAIN:
		break;
	case STATE_GAME:
		drawGameHUD(width, height);
		break;
	case STATE_EDITOR:
		break;
	}
}
