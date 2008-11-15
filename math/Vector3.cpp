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

#include "Vector3.hpp"

#include "functions.hpp"

#include <math.h>

Vector3::Vector3()
{
  // empty
}

Vector3::Vector3(float x, float y, float z) :
	x(x), y(y), z(z)
{
  // empty
}

float Vector3::len() const
{
	return sqrt(sqr(x) + sqr(y) + sqr(z));
}

Vector3 Vector3::norm() const
{
	float l = len();

	Vector3 b(0.0f, 0.0f, 0.0f);

	if (l > 0.0f)
	{
		b.x = x / l;
		b.y = y / l;
		b.z = z / l;
	}

	return b;
}

bool Vector3::operator==(const Vector3& other) const
{
	return x == other.x && y == other.y && z == other.z;
}

Vector3 Vector3::operator*(float s) const
{
	Vector3 b;

	b.x = x * s;
	b.y = y * s;
	b.z = z * s;

	return b;
}

Vector3 Vector3::operator/(float s) const
{
	Vector3 b;

	b.x = x / s;
	b.y = y / s;
	b.z = z / s;

	return b;
}

Vector3 Vector3::operator+(const Vector3& b) const
{
	Vector3 c;

	c.x = x + b.x;
	c.y = y + b.y;
	c.z = z + b.z;

	return c;
}

Vector3 Vector3::operator-(const Vector3& b) const
{
	Vector3 c;

	c.x = x - b.x;
	c.y = y - b.y;
	c.z = z - b.z;

	return c;
}

Vector3 Vector3::operator-() const
{
	Vector3 b;

	b.x = -x;
	b.y = -y;
	b.z = -z;

	return b;
}

float Vector3::operator*(const Vector3& b) const
{
	return x * b.x + y * b.y + z * b.z;
}

Vector3 Vector3::operator^(const Vector3& b) const
{
	Vector3 c;

	c.x = y * b.z - z * b.y;
	c.y = z * b.x - x * b.z;
	c.z = x * b.y - y * b.x;

	return c;
}

void Vector3::operator+=(const Vector3& b)
{
	x += b.x;
	y += b.y;
	z += b.z;
}
