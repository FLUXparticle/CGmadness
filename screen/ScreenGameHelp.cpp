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

#include "ScreenGameHelp.hpp"

#include "MenuManager.hpp"

#include "utils/Singleton.hpp"

#include "keyboard.hpp"

#include "macros.hpp"

typedef struct
{
	char* left;
	char* right;
} LeftRight;

static LeftRight gTextHelp[] =
{
	{"Cursor", "Move"},
	{"Space", "Jump"},
	{"W A S D", "Camera"},
	{"R F", "Zoom"},
	{"Enter", "Reset"},
	{"Esc", "Menu"},
};

static void clickButtonBack()
{
	Singleton<MenuManager> gMenuManager;
	
	gMenuManager->popScreen();
}

ScreenGameHelp::ScreenGameHelp() :
	lTextHelp(2 * LENGTH(gTextHelp))
{
	for (unsigned int i = 0; i < lTextHelp.size(); i++)
	{
		int row = i / 2;
		int col = i % 2;
		float z = 6.0f - row;

		lTextHelp[i] = Label(col ? 5.0f : -5.0f, z, 1.0f, col,
							col ? gTextHelp[row].right : gTextHelp[row].left);
		mItems.push_back(&lTextHelp[i]);
	}

	bBack = Button(6.0f - LENGTH(gTextHelp), clickButtonBack, "back", KEY_ESC);
	mItems.push_back(&bBack);
}

ScreenGameHelp::~ScreenGameHelp()
{
  // empty
}

