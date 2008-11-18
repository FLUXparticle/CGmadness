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

Color4 Color4::white(1.0f, 1.0f, 1.0f);
Color4 Color4::gray(0.5f, 0.5f, 0.5f);
Color4 Color4::red(1.0f, 0.0f, 0.0f);
Color4 Color4::green(0.0f, 1.0f, 0.0f);
Color4 Color4::blue(0.0f, 0.0f, 1.0f);
Color4 Color4::yellow(1.0f, 1.0f, 0.0f);
Color4 Color4::black(0.0f, 0.0f, 0.0f);

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

Color3::operator const float* () const
{
	return &r;
}

Color3 Color3::interpolate(float s, const Color3& other)
{
	float t = 1.0f - s;
	return Color3(s * other.r + t * r, s * other.g + t * g, s * other.b + t * b);
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

Color4::Color4(const Color3& color, float a) :
	Color3(color)
{
	this->a = a;
}

void Color4::operator *= (const Color4& other)
{
	r *= other.r;
	g *= other.g;
	b *= other.b;
	a *= other.a;
}

Color3 interpolateColor(Color3 col1, Color3 col2, float t)
{
	float invT = 1.0f - t;

	return Color3(col1.r * invT + col2.r * t, col1.g * invT + col2.g * t,
								col1.b * invT + col2.b * t);
}
