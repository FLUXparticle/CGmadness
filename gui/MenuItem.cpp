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

#include "MenuItem.hpp"

#define SCALE_FONT 0.5f

float MenuItem::scaleText = 0.1f * SCALE_FONT;

MenuItem::MenuItem(bool interactive) :
	hover(false),
	mInteractive(interactive)
{
  // empty
}

MenuItem::~MenuItem()
{
  // empty
}

void MenuItem::update(float interval)
{
  // empty
}

bool MenuItem::updateSelected(float interval)
{
  return true;
}

void MenuItem::draw() const
{
  // empty
}
