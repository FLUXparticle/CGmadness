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

#include "RaceTheClock.hpp"

#include "objects.hpp"
#include "environment.hpp"

#include "menumanager.hpp"

StringList sgLevels;

void initMain()
{
	createStringListFromDir(&sgLevels, "levels");

	initObjects();
	initEnvironment();

	initGUI();
	
	initMenuManager();
}

void setMainState(MainState newState)
{
	Singleton<Main> gDispenser;
	
	gDispenser->setState(newState);
}

void resumeGame()
{
	Singleton<RaceTheClock> gGameProcess;

	gGameProcess->resumeGame();
}

void resetGame()
{
	Singleton<RaceTheClock> gGameProcess;

	gGameProcess->resetGame();
}

void resetGameTime()
{
	Singleton<RaceTheClock> gGameProcess;

	gGameProcess->resetGameTime();
}
