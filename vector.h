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

#ifndef _vector_h_
#define _vector_h_

#include "types.h"

typedef struct {
	float x;
	float y;
} Vector2;

typedef struct {
	float x;
	float y;
	float z;
} Vector3;

float sqr(float x);

float len(const Vector3 v);

Vector3 norm(const Vector3 v);

Vector3 scale(float s, const Vector3 a);

Vector3 add(const Vector3 a, const Vector3 b);

Vector3 sub(const Vector3 a, const Vector3 b);

Vector3 neg(const Vector3 a);

float dot(const Vector3 a, const Vector3 b);

Vector3 cross(const Vector3 a, const Vector3 b);

Vector3 mkVector3(float x, float y, float z);

void initProjectMat(Matrix m, float fov); 

#endif
