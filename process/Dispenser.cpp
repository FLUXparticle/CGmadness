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
	mNewProcess(NULL)
{
	// empty
}

Dispenser::~Dispenser()
{
	// empty
}

void Dispenser::update(float interval)
{
	if (mNewProcess)
	{
		Process* previous = mProcessStack.top();
		
		if (mPush)
		{
			previous->suspend();
		}
		else
		{
			mProcessStack.pop();
			previous->stop();
		}

		mNewProcess->start(previous);
		mProcessStack.push(mNewProcess);
		mNewProcess = NULL;
	}

	mProcessStack.top()->update(interval);
}

void Dispenser::preDisplay()
{
	mProcessStack.top()->preDisplay();
}

void Dispenser::draw() const
{
	mProcessStack.top()->draw();
}

void Dispenser::drawHUD(float width, float height)
{
	mProcessStack.top()->drawHUD(width, height);
}

void Dispenser::setProcess(Process* process)
{
	if (mProcessStack.empty())
	{
		mProcessStack.push(process);
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
