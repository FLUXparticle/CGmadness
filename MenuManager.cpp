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

#include "MenuManager.hpp"

#include "screen/Screen.hpp"

MenuManager::MenuManager()
{
  // empty
}

MenuManager::~MenuManager()
{
  // empty
}

void MenuManager::pushScreen(Screen* screen)
{
	mScreenStack.push(screen);
	screen->show();
}

void MenuManager::popScreen()
{
	mScreenStack.pop();
	
	if (!mScreenStack.empty())
	{
		mScreenStack.top()->show();
	}
}

void MenuManager::update(float interval)
{
	mScreenStack.top()->update(interval);
}

void MenuManager::event(const Vector3& position, const Vector3& direction, MouseEvent event)
{
	mScreenStack.top()->event(position, direction, event);
}

void MenuManager::draw() const
{
	mScreenStack.top()->draw();
}
