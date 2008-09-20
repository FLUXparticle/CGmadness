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

Dispenser::Task::Task(Process* process, StackAction action) :
	process(process), action(action)
{
	// empty
}

Dispenser::Dispenser()
{
	// empty
}

Dispenser::~Dispenser()
{
	// empty
}

void Dispenser::update(float interval)
{
	FOREACH(mProcessStack, iter)
	{
		Process* p = *iter;
		p->update(interval);
	}

	while (!mTasks.empty())
	{
		Task task = mTasks.front();
		changeProcess(task.process, task.action);
		mTasks.pop_front();
	}
}

void Dispenser::preDisplay()
{
	FOREACH(mProcessStack, iter)
	{
		Process* p = *iter;
		p->preDisplay();
	}
}

void Dispenser::draw() const
{
	FOREACH(mProcessStack, iter)
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
	enqueueTask(process, flush ? FLUSH : SET);
}

void Dispenser::pushProcess(Process* process)
{
	enqueueTask(process, PUSH);
}

void Dispenser::popProcess()
{
	enqueueTask(NULL, POP);
}

void Dispenser::enqueueTask(Process* process, StackAction action)
{
	mTasks.push_back(Task(process, action));
}

void Dispenser::changeProcess(Process* process, StackAction action)
{
	Process* previous = mProcessStack.back();
	switch (action)
	{
	case PUSH:
	{
		previous->suspend();

		Process* next = process;
		mProcessStack.push_back(next);
		next->start(previous, true);
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
		next->start(previous, false);
		break;
	}
	case FLUSH:
	{
		while (!mProcessStack.empty())
		{
			previous = mProcessStack.back();
			mProcessStack.pop_back();
			previous->stop();
		}

		Process* next = process;
		mProcessStack.push_back(next);
		next->start(previous, false);
		break;
	}
	case NONE:
		break;
	}
}
