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

#include "ScreenGameMain2.hpp"

#include "MenuManager.hpp"

#include "utils/Singleton.hpp"

#include "keyboard.hpp"
#include "main.hpp"

static Singleton<MenuManager> gMenuManager;

static void clickButtonResume()
{
	gMenuManager->popScreen();
	resumeGame();
}

ScreenGameMain2::ScreenGameMain2()
{
	bResume = Button(6.0f, clickButtonResume, "resume", KEY_ENTER);
	mItems.push_back(&bResume);
}

ScreenGameMain2::~ScreenGameMain2()
{
  // empty
}

