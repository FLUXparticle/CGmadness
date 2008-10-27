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

#include "Button.hpp"

#include "font3d/font3d.hpp"
#include "hw/keyboard.hpp"

#include GL_H

Button::Button()
{
  // empty
}

Button::Button(float z, const Caller& click, const char* text, int shortcut) :
	mClick(click)
{
	this->text = text;
	this->shortcut = shortcut;

	this->width = widthFont3DText(this->text) * scaleText;
	this->height = 0.5f;

	this->position = Vector2(-this->width / 2.0f, z);
}

Button::~Button()
{
  // empty
}

void Button::update(float interval)
{
	if (wasKeyPressed(shortcut))
	{
		mClick();
	}
}

void Button::updateSelected(float interval)
{
	if (wasKeyPressed(KEY_ENTER))
	{
		mClick();
	}
}

void Button::draw() const
{
	float scale = scaleText * (1.0f + 0.1f * this->emphasize);

	glPushMatrix();
	{
		glTranslatef(-0.5f * this->width * 0.1f * this->emphasize, 0.0f,
				0.0f);
		glScalef(scale, scale, scale);

		drawFont3DText(this->text);
	}
	glPopMatrix();
}


