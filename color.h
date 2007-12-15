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

#ifndef _color_h_
#define _color_h_

typedef struct
{
	float r;
	float g;
	float b;
} Color3;

typedef struct
{
	float r;
	float g;
	float b;
	float a;
} Color4;

Color4 color4(float r, float g, float b, float a);

Color3 color3(float r, float g, float b);
Color3 color3i(int r, int g, int b);

Color3 interpolateColor(Color3 col1, Color3 col2, float t);

#endif
