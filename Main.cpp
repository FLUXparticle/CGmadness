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

#include "gui/gui.hpp"
#include "ball/BallObject.hpp"

#include "Main.hpp"

void Main::init()
{
	BallObject::init();
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

void Main::setFirstProcess(Process* first)
{
	changeProcess(first, FLUSH);
}

void Main::setState(Process* process, bool flush)
{
	Singleton<Main> gDispenser;
	
	gDispenser->setProcess(process, flush);
}

void Main::pushState(Process* process)
{
	Singleton<Main> gDispenser;
	
	gDispenser->pushProcess(process);
}

void Main::popState()
{
	Singleton<Main> gDispenser;
	
	gDispenser->popProcess();
}
