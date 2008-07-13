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

#include "SpinEdit.hpp"

#include "objects.hpp"
#include "texture.hpp"
#include "hw/keyboard.hpp"

#include "functions.hpp"

#include <GL/gl.h>

/*
 * TODO split SpinEdit into two seperate MenuItems
 */

static unsigned int gTexLeft;
static unsigned int gTexRight;

void drawArrowLeft(void)
{
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, gTexLeft);
	{
		glPushMatrix();
		{
			glScalef(0.5f, 0.5f, 1.0f);

			drawSquare();
		}
		glPopMatrix();
	}
	glDisable(GL_TEXTURE_2D);
}

void drawArrowRight(void)
{
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, gTexRight);
	{
		glPushMatrix();
		{
			glScalef(0.5f, 0.5f, 1.0f);

			drawSquare();
		}
		glPopMatrix();
	}
	glDisable(GL_TEXTURE_2D);
}

void SpinEdit::init()
{
	/* loading arrow textures */
	gTexLeft = loadTexture("data/left.tga", false);
	gTexRight = loadTexture("data/right.tga", false);
}

SpinEdit::SpinEdit()
{
	// empty
}

SpinEdit::SpinEdit(int value, int min, int max, float width, float z, funcDraw draw, funcChange change)
{
	this->width = width;
	this->height = 1.0f;

	this->position = Vector2(-this->width / 2.0f, z - 0.5);

	this->value = value;
	this->minValue = min;
	this->maxValue = max;

	/* object between arrows */
	this->fDraw = draw;
	this->fChange = change;

	this->fChange(this);
}

SpinEdit::~SpinEdit()
{
	// empty
}

void SpinEdit::change(int change)
{
	this->value = clamp(this->value + change, this->minValue, this->maxValue);
	this->fChange(this);
}

void SpinEdit::event(float x, float y, MouseEvent event)
{
	int side;

	float dist = this->width / 2.0f - 1.0f;

	if (x < -dist)
	{
		side = -1;
	}
	else if (x > dist)
	{
		side = 1;
	}
	else
	{
		side = 0;
	}

	if (side != 0)
	{
		this->side = side;
	}

	switch (event)
	{
	case MOUSE_CLICK:
		change(side);
		break;
	default:
		this->hover = (side != 0);
		break;
	}
}

void SpinEdit::update(float interval)
{
	if (wasCursorPressed(CURSOR_LEFT))
	{
		this->change(-1);
	}
	else if (wasCursorPressed(CURSOR_RIGHT))
	{
		this->change(+1);
	}
}

void SpinEdit::draw() const
{
	float scale = 1.0f + 0.2f * this->emphasize;
	float step = (this->width - 1.0f) / 2.0f;

	glPushMatrix();
	{
		glTranslatef(0.5f, 0.5f, 0.0f);

		glPushMatrix();
		{
			if (this->side == -1)
			{
				glScalef(scale, scale, 1.0f);
			}

			drawArrowLeft();
		}
		glPopMatrix();

		glTranslatef(step, 0.0f, 0.0f);

		glPushMatrix();
		{
			glScalef(0.5f, 0.5f, 0.5f);

			glPushAttrib(GL_CURRENT_BIT | GL_LIGHTING_BIT);
			{
				this->fDraw();
			}
			glPopAttrib();
		}
		glPopMatrix();

		glTranslatef(step, 0.0f, 0.0f);

		glPushMatrix();
		{
			if (this->side == 1)
			{
				glScalef(scale, scale, 1.0f);
			}

			drawArrowRight();
		}
		glPopMatrix();
	}
	glPopMatrix();
}
