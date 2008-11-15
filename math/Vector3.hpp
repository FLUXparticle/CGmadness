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

#ifndef Vector3_hpp
#define Vector3_hpp

struct Vector3
{
	float x;
	float y;
	float z;

	Vector3();
  Vector3(float x, float y, float z);

  float len() const;
  Vector3 norm() const;

  bool operator == (const Vector3& other) const;

  operator const float* () const;

  Vector3 operator*(float s) const;
  Vector3 operator/(float s) const;
  Vector3 operator+(const Vector3& b) const;
  Vector3 operator-(const Vector3& b) const;
  Vector3 operator-() const;
  float operator*(const Vector3& b) const;
  Vector3 operator^(const Vector3& b) const;

  void operator+=(const Vector3& b);
};

inline Vector3::operator const float* () const
{
	return &x;
}

inline float len(const Vector3 v)
{
	return v.len();
}

inline Vector3 norm(const Vector3 v)
{
	return v.norm();
}

inline Vector3 neg(const Vector3 a)
{
	return -a;
}

inline Vector3 scale(float s, const Vector3 a)
{
	return a * s;
}

inline Vector3 add(const Vector3 a, const Vector3 b)
{
	return a + b;
}

inline Vector3 sub(const Vector3 a, const Vector3 b)
{
	return a - b;
}

inline float dot(const Vector3 a, const Vector3 b)
{
	return a * b;
}

inline Vector3 cross(const Vector3 a, const Vector3 b)
{
	return a ^ b;
}

#endif
