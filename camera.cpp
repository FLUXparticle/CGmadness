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

#include "camera.hpp"

#include "callback.hpp"

#include <GL/glu.h>

Vector3 sgCamera;
Vector3 sgLookat;
static Vector3 gUp(0.0f, 0.0f, 1.0f);

void resetCamera(void)
{
	sgCamera.x = 0.0f;
	sgCamera.y = 0.0f;
	sgCamera.z = 1.0f;

	sgLookat.x = 0.0f;
	sgLookat.y = 0.0f;
	sgLookat.z = 0.0f;
}

void moveCamera(float interval, Vector3 camera, Vector3 lookat)
{
	Vector3 diff;
	float error;

	/* new values */
	diff = sub(camera, sgCamera);
	error = len(diff);
	sgCamera = add(sgCamera, scale(5.0f * interval * error, norm(diff)));

	diff = sub(lookat, sgLookat);
	error = len(diff);
	sgLookat = add(sgLookat, scale(5.0f * interval * error, norm(diff)));
}

void setCamera()
{
	gluLookAt(sgCamera.x, sgCamera.y, sgCamera.z,
						sgLookat.x, sgLookat.y, sgLookat.z,
						gUp.x, gUp.y, gUp.z);
}

Vector3 rotateVector(const Vector3& dir)
{
	Vector3 direction;
	
	Vector3 f = (sgCamera - sgLookat).norm();
	Vector3 s = gUp ^ f;
	Vector3 u = f ^ s;
	
	direction.x = dir.x * s.x + dir.y * u.x + dir.z * f.x;
	direction.y = dir.x * s.y + dir.y * u.y + dir.z * f.y;
	direction.z = dir.x * s.z + dir.y * u.z + dir.z * f.z;
	
	return direction;
}
