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

#include "Check.hpp"

#include "font3d/font3d.hpp"

#include <GL/glew.h>

Check::Check()
{
	// empty
}

Check::Check(float z, funcChange change, const char* text)
{
	this->text = text;
	this->change = change;

	this->width = widthFont3DText(this->text) * scaleText;
	this->height = 0.5f;

	this->position = Vector2(-this->width / 2.0f, z);

	set(true);
}

Check::~Check()
{
	// empty
}

void Check::set(bool value)
{
	this->value = value;
	this->change(this);
}

void Check::event(float x, float y, MouseEvent event)
{
	switch (event)
	{
	case MOUSE_CLICK:
		set(!this->value);
		break;
	default:
		this->hover = 1;
		break;
	}
}

void Check::draw() const
{
	float scale = scaleText * (1.0f + 0.1f * this->emphasize);

	glPushMatrix();
	{
		glTranslatef(-0.5f * this->width * 0.1f * this->emphasize, 0.0f,
				0.0f);
		glScalef(scale, scale, scale);

		if (this->value)
		{
			glColor3f(1.0f, 1.0f, 1.0f);
		}
		else
		{
			glColor3f(0.5f, 0.5f, 0.5f);
		}

		drawFont3DText(this->text);

		glColor3f(1.0f, 1.0f, 1.0f);
	}
	glPopMatrix();
}
