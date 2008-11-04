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

#include "ScreenHelp.hpp"

#include "utils/Callback.hpp"

#include "hw/keyboard.hpp"

ScreenHelp::ScreenHelp(unsigned int length, LeftRight gTextHelp[], float size) :
	lTextHelp(2 * length)
{
	for (unsigned int i = 0; i < lTextHelp.size(); i++)
	{
		int row = i / 2;
		int col = i % 2;
		float z = 6.0f - row * size;

		lTextHelp[i] = Label(col ? 5.0f : -5.0f, z, size, col,
							col ? gTextHelp[row].right : gTextHelp[row].left);
		addItem(&lTextHelp[i]);
	}

	bBack = Button(6.0f - length * size, METHOD_CALLBACK(ScreenHelp, clickButtonBack), "back", KEY_ESC);
	addItem(&bBack);
}

ScreenHelp::~ScreenHelp()
{
  // empty
}

void ScreenHelp::clickButtonBack()
{
	popScreen();
}
