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

#include "vector.h"

#include "functions.h"

#include <math.h>

/*
 * some vector calculations
 */

float len(Vector3 v) {
	return sqrt(sqr(v.x) + sqr(v.y) + sqr(v.z));
}

void normalize(Vector3* v) {
	float l = len(*v);
	if (l > 0.0f) {
		v->x /= l;
		v->y /= l;
		v->z /= l;
	}
}

Vector3 norm(Vector3 v) {
	normalize(&v);
	return v;
}

Vector3 scale(float s, Vector3 a) {
	Vector3 b;

	b.x = s * a.x;
	b.y = s * a.y;
	b.z = s * a.z;

	return b;
}

Vector3 add(Vector3 a, Vector3 b) {
	Vector3 c;

	c.x = a.x + b.x;
	c.y = a.y + b.y;
	c.z = a.z + b.z;

	return c;
}

Vector3 sub(Vector3 a, Vector3 b) {
	Vector3 c;

	c.x = a.x - b.x;
	c.y = a.y - b.y;
	c.z = a.z - b.z;

	return c;
}

Vector3 neg(Vector3 a) {
	Vector3 b;

	b.x = -a.x;
	b.y = -a.y;
	b.z = -a.z;

	return b;
}

float dot(Vector3 a, Vector3 b) {
	return a.x * b.x + a.y * b.y + a.z * b.z;
}

Vector3 cross(Vector3 a, Vector3 b) {
	Vector3 c;

	c.x = a.y * b.z - b.y * a.z;
	c.y = b.x * a.z - a.x * b.z;
	c.z = a.x * b.y - b.x * a.y;

	return c;
}

Vector3 mkVector3(float x, float y, float z) {
	Vector3 v;
	
	v.x = x;
	v.y = y;
	v.z = z;
	
	return v;
}

/*
 * projection matrix without farplane
 */
void initProjectMat(Matrix m, float fov) {
	int x;
	int y;
	float f = 1.0f / tan(fov / 2.0f * PI / 180.0f);

	for (x = 0; x < 4; x++) {
		for (y = 0; y < 4; y++) {
			m[x][y] = 0.0f;
		}
	}

	m[0][0] = f;
	
	m[1][1] = f;
	
	m[2][2] = -1;
	m[2][3] = -1;
	
	m[3][2] = -2 * 0.01f;
}
