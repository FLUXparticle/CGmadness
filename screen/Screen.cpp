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

#include "texture.hpp"
#include "camera.hpp"
#include "objects.hpp"

#include "macros.hpp"

#include <GL/glut.h>

#define EMPHASIZE_SPEED 10.0f

unsigned int Screen::gTexLogo;

Screen::Screen()
{
	if (!gTexLogo)
	{
		gTexLogo = loadTexture("data/logo.tga", false);
	}
}

Screen::~Screen()
{
  // empty
}

void Screen::start(Process* previous)
{
	mPrevious = previous;
	show();
}

void Screen::show()
{
  FOREACH(mItems, iter)
	{
		(*iter)->emphasize = 0.0f;
	}

  (*mSelectedItem)->hover = true;

	glutSetCursor(GLUT_CURSOR_LEFT_ARROW);
}

void Screen::popScreen()
{
	Main::setState(mPrevious, false);
}

void Screen::update(float interval)
{
	Vector3 camera = Vector3(0.0f, -10.0f, 7.0f);
	Vector3 lookat = Vector3(0.0f, 0.0f, 5.0f);

	moveCamera(interval, camera, lookat);

  FOREACH(mItems, iter)
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
	float pos[4] = { 0.0f, -1.0f, 0.5f, 0.0f };

	glEnable(GL_LIGHT0);
	glLightfv(GL_LIGHT0, GL_POSITION, pos);

	glEnable(GL_LIGHTING);
	glEnable(GL_COLOR_MATERIAL);

	glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);
	{
		glColor3f(1.0f, 1.0f, 1.0f);

		glPushMatrix();
		{
			glRotatef(90.0f, 1.0f, 0.0f, 0.0f);

			drawLogo();
		  FOREACH(mItems, iter)
		  {
		  	drawMenuItem(*iter);
		  }
		}
		glPopMatrix();
	}
	glDisable(GL_COLOR_MATERIAL);
	glDisable(GL_LIGHTING);
	glDisable(GL_LIGHT0);
}

void Screen::addItem(MenuItem* item)
{
	mItems.push_back(item);

	if (item->interactive())
	{
		MenuItems::iterator pos = mInteractiveItems.end();
		FOREACH(mInteractiveItems, iter)
		{
			if ((*iter)->position.y < item->position.y)
			{
				pos = iter;
				break;
			}
		}

		mInteractiveItems.insert(pos, item);
		mSelectedItem = mInteractiveItems.begin();
	}
}

void Screen::drawLogo()
{
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, gTexLogo);
	{
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		{
			glPushMatrix();
			{
				glTranslatef(0.0f, 8.0f, 0.0f);
				glScalef(4.0f, 1.0f, 1.0f);

				drawSquare();
			}
			glPopMatrix();
		}
		glDisable(GL_BLEND);
	}
	glDisable(GL_TEXTURE_2D);
}
