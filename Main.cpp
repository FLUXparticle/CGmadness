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

#include "main.hpp"

#include "ball.hpp"
#include "environment.hpp"

#include "menumanager.hpp"

#include "Main.hpp"
#include "Game.hpp"
#include "Editor.hpp"

StringList sgLevels;

static Process* gCurProcess;

static Main gMainProcess;
static Game gGameProcess;
static Editor gEditorProcess;

void initMain(void)
{
	createStringListFromDir(&sgLevels, "levels");

	initEnvironment();

	initMenuManager();
	
	gMainProcess.init();
	setMainState(STATE_MAIN);

	gGameProcess.init();
	gEditorProcess.init();
}

void setProcess(Process* process)
{
	if (gCurProcess)
	{
		gCurProcess->stop();
	}
	
	gCurProcess = process;
	
	if (gCurProcess)
	{
		gCurProcess->start();
	}
}

void setMainState(MainState newState)
{
	switch (newState)
	{
	case STATE_MAIN:
		setProcess(&gMainProcess);
		break;
	case STATE_GAME:
		setProcess(&gGameProcess);
		break;
	case STATE_EDITOR:
		setProcess(&gEditorProcess);
		break;
	}
}

void updateMain(float interval)
{
	gCurProcess->update(interval);
}

void drawMain()
{
	gCurProcess->draw();
}

void drawMainHUD(float width, float height)
{
	gCurProcess->drawHUD(width, height);
}
