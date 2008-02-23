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

#ifndef _gui_hpp_
#define _gui_hpp_

#include "gui/MenuItem.hpp"

#include "math/Vector2.hpp"
#include "mouse.hpp"
#include "types.hpp"

/***/

typedef struct Screen
{
	int cntItems;
	MenuItem **items;

	struct Screen *back;
} Screen;

void initGUI(void);

/* Screen */

#define INIT_SCREEN(screen, items) initScreen((screen), LENGTH(items), (items))

void initScreen(Screen * screen, int cntItems, MenuItem ** items);
void prepareScreen(Screen * screen);
void updateScreen(Screen * screen, float interval);
void drawScreen(const Screen * screen);
void eventScreen(Screen * screen, float x, float y, MouseEvent event);

#endif
