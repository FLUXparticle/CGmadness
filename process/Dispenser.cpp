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
	mIsUpdating(false), mNewProcess(NULL), mAction(NONE)
{
	// empty
}

Dispenser::~Dispenser()
{
	// empty
}

void Dispenser::event(const Vector3& position, const Vector3& direction,
		MouseEvent event)
{
	mProcessStack.back()->event(position, direction, event);
}

void Dispenser::update(float interval)
{
	if (mAction != NONE)
	{
		changeProcess(mNewProcess, mAction);
		mAction = NONE;
	}

	mIsUpdating = true;

	FOREACH(std::list<Process*>, mProcessStack, iter)
	{
		Process* p = *iter;
		p->update(interval);
	}

	mIsUpdating = false;
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

void Dispenser::setProcess(Process* process, bool flush)
{
	changeProcess(process, flush ? FLUSH : SET);
}

void Dispenser::pushProcess(Process* process)
{
	changeProcess(process, PUSH);
}

void Dispenser::popProcess()
{
	changeProcess(NULL, POP);
}

void Dispenser::popScreen()
{
	Process* process = mProcessStack.back();
	Screen* screen = dynamic_cast<Screen*>(process);
	screen->popScreen();
}

void Dispenser::changeProcess(Process* process, StackAction action)
{
	if (mIsUpdating)
	{
		mNewProcess = process;
		mAction = action;
	}
	else
	{
		Process* previous = mProcessStack.back();
		switch (action)
		{
		case PUSH:
		{
			previous->suspend();

			Process* next = process;
			mProcessStack.push_back(next);
			next->start(previous);
			break;
		}
		case POP:
		{
			mProcessStack.pop_back();
			previous->stop();

			Process* next = mProcessStack.back();
			next->resume();
			break;
		}
		case SET:
		{
			mProcessStack.pop_back();
			previous->stop();

			Process* next = process;
			mProcessStack.push_back(next);
			next->start(previous);
			break;
		}
		case FLUSH:
		{
			while (!mProcessStack.empty())
			{
				mProcessStack.pop_back();
				previous->stop();
				previous = mProcessStack.back();
			}

			Process* next = process;
			mProcessStack.push_back(next);
			next->start(previous);
			break;
		}
		case NONE:
			break;
		}
	}
}
