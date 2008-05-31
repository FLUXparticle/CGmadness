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

#include "Screen.hpp"

#include "macros.hpp"

#include <GL/glut.h>

#define EMPHASIZE_SPEED 10.0f

Screen::Screen()
{
  // empty
}

Screen::~Screen()
{
  // empty
}

void Screen::show()
{
  FOREACH(std::list<MenuItem*>, mItems, iter)
	{
		(*iter)->emphasize = 0.0f;
	}

	glutSetCursor(GLUT_CURSOR_LEFT_ARROW);
}

void Screen::event(float x, float y, MouseEvent event)
{
  FOREACH(std::list<MenuItem*>, mItems, iter)
  {
  	MenuItem* item = *iter;
  	
  	item->hover = 0;

  	if (x >= item->position.x && y >= item->position.y &&
  			x <= item->position.x + item->width &&
  			y <= item->position.y + item->height)
  	{
  		item->event(x, y, event);
  	}
  }
}

void Screen::update(float interval)
{
  FOREACH(std::list<MenuItem*>, mItems, iter)
  {
  	MenuItem* item = *iter;
  	
  	if (item->hover)
  	{
  		item->emphasize += EMPHASIZE_SPEED * interval * (1.0f - item->emphasize);
  	}
  	else
  	{
  		item->emphasize += EMPHASIZE_SPEED * interval * (0.0f - item->emphasize);
  	}

  	item->update(interval);
  }
  
  customUpdate(interval);
}

void Screen::customUpdate(float interval)
{
	// empty
}

static void drawMenuItem(const MenuItem* item)
{
	glPushMatrix();
	{
		glTranslatef(item->position.x, item->position.y, 0.0f);

		item->draw();
	}
	glPopMatrix();
}

void Screen::draw() const
{
  FOREACH(std::list<MenuItem*>, mItems, iter)
  {
  	drawMenuItem(*iter);
  }
}

void Screen::drawBackground() const
{
	// empty
}
