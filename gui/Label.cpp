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

#include "Label.hpp"

#include "text/text.hpp"

Label::Label() :
	MenuItem(false)
{
  // empty
}

Label::Label(float x, float z, float size, bool alignRight, const char* text) :
	MenuItem(false)
{
	this->text = text;
	this->size = size;

	this->height = 0.5f * size;
	this->width = widthStrokeText(this->text) * height;

	if (alignRight)
	{
		this->position = Vector2(x - width, z);
	}
	else
	{
		this->position = Vector2(x, z);
	}
}

Label::~Label()
{
  // empty
}

void Label::draw() const
{
	float scale = height;

	glPushMatrix();
	{
		glScalef(scale, scale, scale);

		drawStrokeThickText(text);
	}
	glPopMatrix();
}

