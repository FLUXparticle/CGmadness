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

#include "Canvas.hpp"

Canvas::Canvas(bool interactive) :
	MenuItem(interactive)
{
  // empty
}

Canvas::Canvas(float z, float width, float height, bool interactive) :
	MenuItem(interactive)
{
	this->width = width;
	this->height = height;

	this->position = Vector2(-this->width / 2.0f, z);
}

Canvas::~Canvas()
{
  // empty
}

void Canvas::update(float interval)
{
  // empty
}

void Canvas::draw() const
{
  // empty
}
