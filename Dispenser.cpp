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

#include "Dispenser.hpp"

#include "MenuManager.hpp"

#include "utils/Singleton.hpp"

#include <stdlib.h>

Dispenser::Dispenser() :
	mCurProcess(NULL)
{
  // empty
}

Dispenser::~Dispenser()
{
  // empty
}

void Dispenser::update(float interval)
{
	mCurProcess->update(interval);
}

void Dispenser::event(const Vector3& position, const Vector3& direction, MouseEvent event)
{
#if 0
	mCurProcess->event(position, direction, event);
#else
	Singleton<MenuManager> menuManager;
	
	menuManager->event(position, direction, event);
#endif
}

void Dispenser::preDisplay()
{
	mCurProcess->preDisplay();
}

void Dispenser::draw()
{
	mCurProcess->draw();
}

void Dispenser::drawHUD(float width, float height)
{
	mCurProcess->drawHUD(width, height);
}

void Dispenser::setProcess(Process* process)
{
	if (mCurProcess)
	{
		mCurProcess->stop();
	}
	
	mCurProcess = process;
	
	if (mCurProcess)
	{
		mCurProcess->start();
	}
}
