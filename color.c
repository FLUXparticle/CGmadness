/*
 * CG Madness - a Marble Madness clone
 * Copyright (C) 2007  Sven Reinck
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
 *
 * $Id$
 *
 */

#include "color.h"

Color4 color4(float r, float g, float b, float a)
{
	Color4 col;

	col.r = r;
	col.g = g;
	col.b = b;
	col.a = a;

	return col;
}

Color3 color3(float r, float g, float b)
{
	Color3 col;

	col.r = r;
	col.g = g;
	col.b = b;

	return col;
}

Color3 color3i(int r, int g, int b)
{
	return color3((float) r / 255, (float) g / 255, (float) b / 255);
}

Color3 interpolateColor(Color3 col1, Color3 col2, float t)
{
	float invT = 1.0f - t;

	return color3(col1.r * invT + col2.r * t, col1.g * invT + col2.g * t, col1.b * invT + col2.b * t);
}
