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

#include "Main.hpp"

#include "ball/Ball.hpp"
#include "ball/PlayersBall.hpp"

#include "environment/environment.hpp"

#include "process/MainProcess.hpp"
#include "process/RaceTheClock.hpp"
#include "process/Editor.hpp"

#include "gui/gui.hpp"
#include "objects.hpp"

#include "MenuManager.hpp"

void Main::init()
{
	initObjects();
	initEnvironment();

	initGUI();
	
	Ball::init();
	PlayersBall::init();
}

Main::Main()
{
  // empty
}

Main::~Main()
{
  // empty
}

void Main::event(const Vector3& position, const Vector3& direction, MouseEvent event)
{
	gMenuManager->event(position, direction, event);
}

void Main::setState(MainState newState)
{
	Singleton<Main> gDispenser;
	
	switch (newState)
	{
	case STATE_MAIN:
		gDispenser->setProcess(gDispenser->gMainProcess);
		break;
	case STATE_GAME:
		gDispenser->setProcess(gDispenser->gGameProcess);
		break;
	case STATE_EDITOR:
		gDispenser->setProcess(gDispenser->gEditorProcess);
		break;
	}
}
