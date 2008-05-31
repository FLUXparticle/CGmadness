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

#include "Main.hpp"

#include "screen/Screen.hpp"

#include "utils/Singleton.hpp"

#include "macros.hpp"

#include <stdlib.h>

Dispenser::Dispenser() :
	mNewProcess(NULL)
{
	// empty
}

Dispenser::~Dispenser()
{
	// empty
}

void Dispenser::event(const Vector3& position, const Vector3& direction, MouseEvent event)
{
	mProcessStack.back()->event(position, direction, event);
}

void Dispenser::update(float interval)
{
	if (mNewProcess != mProcessStack.back())
	{
		if (mNewProcess)
		{
			Process* previous = mProcessStack.back();
			Process* next = mNewProcess;

			if (mPush)
			{
				previous->suspend();
			}
			else
			{
				mProcessStack.pop_back();
				previous->stop();
			}

			next->start(previous);
			mProcessStack.push_back(next);
		}
		else
		{
			Process* current = mProcessStack.back();
			mProcessStack.pop_back();
			current->stop();

			mProcessStack.back()->resume();
		}
	}

	FOREACH(std::list<Process*>, mProcessStack, iter)
	{
		Process* p = *iter;
		p->update(interval);
	}
}

void Dispenser::preDisplay()
{
	FOREACH(std::list<Process*>, mProcessStack, iter)
	{
		Process* p = *iter;
		p->preDisplay();
	}
}

void Dispenser::draw() const
{
	FOREACH(std::list<Process*>, mProcessStack, iter)
	{
		Process* p = *iter;
		p->draw();
	}
}

void Dispenser::drawHUD(float width, float height)
{
	mProcessStack.back()->drawHUD(width, height);
}

void Dispenser::setProcess(Process* process)
{
	if (mProcessStack.empty())
	{
		mProcessStack.push_back(process);
		mNewProcess = process;
		process->start(NULL);
	}
	else
	{
		mNewProcess = process;
		mPush = false;
	}
}

void Dispenser::pushProcess(Process* process)
{
	mNewProcess = process;
	mPush = true;
}

void Dispenser::popProcess()
{
	mNewProcess = NULL;
}

void Dispenser::popScreen()
{
	Process* process = mProcessStack.back();
	Screen* screen = dynamic_cast<Screen*>(process); 
	screen->popScreen();
}
