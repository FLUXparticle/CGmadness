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

#include "MainProcess.hpp"

#include "screen/main/ScreenMain.hpp"

#include "Main.hpp"

#include "environment/environment.hpp"

MainProcess::MainProcess()
{
  // empty
}

MainProcess::~MainProcess()
{
  // empty
}

void MainProcess::start(Process* previous, bool push)
{
	Main::pushState(gScreenMain);
}

void MainProcess::update(float interval)
{
	updateEnvironment(interval);
}

void MainProcess::draw() const
{
	drawEnvironment(NULL);
}
