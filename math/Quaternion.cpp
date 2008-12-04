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

#include "Quaternion.hpp"

#include "functions.hpp"

#include GL_H

#include <cmath>

Quaternion::Quaternion()
{
	// empty
}

Quaternion::Quaternion(float alpha, const Vector3& v)
{
	float a = alpha * M_PI / 360.0f;
	float s = sin(a);
	float c = cos(a);

	this->mS = c;
	this->mV = v.norm() * s;
}

float Quaternion::len() const
{
	return sqrt(sqr(mS) + sqr(mV.len()));
}

Quaternion Quaternion::norm() const
{
	Quaternion q;

	float f = 1.0f / q.len();

	q.mS = mS * f;
	q.mV = mV * f;

	return q;
}

void Quaternion::transform() const
{
	float m[16];
	float s = mS;
	float x = mV.x;
	float y = mV.y;
	float z = mV.z;

	m[0] = 1.0f - 2.0f * (y * y + z * z);
	m[1] = 2.0f * (s * z + x * y);
	m[2] = 2.0f * (-s * y + x * z);
	m[3] = 0.0f;

	m[4] = 2.0f * (-s * z + x * y);
	m[5] = 1.0f - 2.0f * (x * x + z * z);
	m[6] = 2.0f * (s * x + y * z);
	m[7] = 0.0f;

	m[8] = 2.0f * (s * y + x * z);
	m[9] = 2.0f * (-s * x + y * z);
	m[10] = 1.0f - 2.0f * (x * x + y * y);
	m[11] = 0.0f;

	m[12] = 0.0f;
	m[13] = 0.0f;
	m[14] = 0.0f;
	m[15] = 1.0f;

	glMultMatrixf(m);
}

Quaternion Quaternion::interpolate(float t, Quaternion b) const
{
	const Quaternion& a = *this;

	float w = acos(a.norm() * b.norm());
	float s = sin(w);

	return a * (sin((1.0f - t) * w) / s) + b * (sin(t * w) / s);
}

Quaternion Quaternion::operator+ (Quaternion b) const
{
	Quaternion c;

	c.mS = mS + b.mS;
	c.mV = mV + b.mV;

	return c;
}

Quaternion Quaternion::operator- (Quaternion b) const
{
	Quaternion c;

	c.mS = mS - b.mS;
	c.mV = mV - b.mV;

	return c;
}

float Quaternion::operator* (Quaternion b) const
{
	return mS * b.mS + mV * b.mV;
}

Quaternion Quaternion::operator^ (Quaternion b) const
{
	Quaternion c;

	c.mS = mS * b.mS - mV * b.mV;
	c.mV = ((mV ^ b.mV) + ((mS * b.mV) + (mV * b.mS)));

	return c;
}

Quaternion Quaternion::operator* (float f) const
{
	Quaternion q;

	q.mS = mS * f;
	q.mV = mV * f;

	return q;
}
