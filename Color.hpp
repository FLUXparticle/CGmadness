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

#ifndef Color_hpp
#define Color_hpp

struct Color3
{
	Color3();
	Color3(float r, float g, float b);

	operator const float* () const;

	Color3 interpolate(float s, const Color3& other);

	float r;
	float g;
	float b;
};

struct Color4 : public Color3
{
public:
	static Color4 white;
	static Color4 gray;
	static Color4 red;
	static Color4 green;
	static Color4 blue;
	static Color4 yellow;
	static Color4 black;

public:
	Color4();
	Color4(const Color3& color, float a = 1.0f);
	Color4(float r, float g, float b, float a = 1.0f);

	void operator *= (const Color4& other);

	float a;
};

Color3 interpolateColor(Color3 col1, Color3 col2, float t);

#endif
