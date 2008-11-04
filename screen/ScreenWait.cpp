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

#include "ScreenWait.hpp"

#include "idle.hpp"

#include "hw/keyboard.hpp"
#include "callback.hpp"

ScreenWait::ScreenWait(const Caller& callback) :
	gWaitCallback(callback)
{
	pbProgress = ProgressBar(5.0f, &sgIdleProgress);
	addItem(&pbProgress);
}

ScreenWait::~ScreenWait()
{
  // empty
}

void ScreenWait::customUpdate(float interval)
{
	if (wasKeyPressed(KEY_ESC) || wasKeyPressed('q'))
	{
		stopIdle();
		
		setUpdateFrequency(0);
		popScreen();
	}
	else if (!sgIdleWorking)
	{
		setUpdateFrequency(0);
		popScreen();

		gWaitCallback();
	}
}
