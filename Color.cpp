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

#include "Color.hpp"

Color3::Color3()
{
	// empty
}

Color3::Color3(float r, float g, float b)
{
	this->r = r;
	this->g = g;
	this->b = b;
}

Color4::Color4()
{
	// empty
}

Color4::Color4(float r, float g, float b, float a) :
	Color3(r, g, b)
{
	this->a = a;
}

Color3 interpolateColor(Color3 col1, Color3 col2, float t)
{
	float invT = 1.0f - t;

	return Color3(col1.r * invT + col2.r * t, col1.g * invT + col2.g * t,
								col1.b * invT + col2.b * t);
}
