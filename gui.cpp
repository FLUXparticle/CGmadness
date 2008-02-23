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

#include "gui.hpp"

#include "gui/SpinEdit.hpp"

#include "text.hpp"
#include "objects.hpp"
#include "texture.hpp"
#include "keyboard.hpp"

#include "functions.hpp"

#include <GL/glut.h>

#include <stdio.h>
#include <string.h>

/*** common ***/

void initGUI()
{
	SpinEdit::init();
}

/*** Canvas ***/

void initCanvas(Canvas * canvas, float z, float width, float height,
								funcUpdate customUpdate, funcDraw customDraw)
{
	canvas->customUpdate = customUpdate;
	canvas->customDraw = customDraw;

	canvas->item.type = MI_CANVAS;

	canvas->item.width = width;
	canvas->item.height = height;

	canvas->item.position = Vector2(-canvas->item.width / 2.0f, z);
}

void drawCanvas(const Canvas * canvas)
{
	canvas->customDraw();
}

/*** MenuItem ***/

#define EMPHASIZE_SPEED 10.0f

void updateMenuItem(MenuItem * item, float interval)
{
	if (item->hover)
	{
		item->emphasize += EMPHASIZE_SPEED * interval * (1.0f - item->emphasize);
	}
	else
	{
		item->emphasize += EMPHASIZE_SPEED * interval * (0.0f - item->emphasize);
	}

	/*
	 * TODO  needs rewrite of event-system
	 */
	if (item->type == MI_BUTTON)
	{
		item->update(interval);
	}
	else if (item->type == MI_SPIN_EDIT)
	{
		item->update(interval);
	}
	else if (item->type == MI_CANVAS)
	{
		Canvas *canvas = (Canvas *) item;
		canvas->customUpdate(interval);
	}
}

void drawMenuItem(const MenuItem * item)
{
	glPushMatrix();

	glTranslatef(item->position.x, item->position.y, 0.0f);

	switch (item->type)
	{
	case MI_CANVAS:
		drawCanvas((const Canvas *) item);
		break;
	case MI_LABEL:
		item->draw();
		break;
	case MI_PROGRESS_BAR:
		item->draw();
		break;
	case MI_BUTTON:
		item->draw();
		break;
	case MI_CHECK:
		item->draw();
		break;
	case MI_SPIN_EDIT:
		item->draw();
		break;
	}

	glPopMatrix();
}

void eventMenuItem(MenuItem * item, float x, float y, MouseEvent event)
{
	item->hover = 0;

	if (x >= item->position.x && y >= item->position.y &&
			x <= item->position.x + item->width
			&& y <= item->position.y + item->height)
	{
		item->event(x, y, event);
	}
}

/*** Screen ***/

void initScreen(Screen * screen, int cntItems, MenuItem ** items)
{
	screen->cntItems = cntItems;
	screen->items = items;
}

void prepareScreen(Screen * screen)
{
	int i;

	for (i = 0; i < screen->cntItems; i++)
	{
		screen->items[i]->emphasize = 0.0f;
	}
}

void updateScreen(Screen * screen, float interval)
{
	int i;

	for (i = 0; i < screen->cntItems; i++)
	{
		updateMenuItem(screen->items[i], interval);
	}
}

void drawScreen(const Screen * screen)
{
	int i;

	for (i = 0; i < screen->cntItems; i++)
	{
		drawMenuItem(screen->items[i]);
	}
}

void eventScreen(Screen * screen, float x, float y, MouseEvent event)
{
	int i;

	for (i = 0; i < screen->cntItems; i++)
	{
		eventMenuItem(screen->items[i], x, y, event);
	}
}
