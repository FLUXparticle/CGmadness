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

#include "camera.h"

#include "callback.h"

#include <GL/glu.h>

Vector3 sgCamera;
Vector3 sgLookat;

void resetCamera(void) {
	sgCamera.x = 0.0f;
	sgCamera.y = 0.0f;
	sgCamera.z = 1.0f;

	sgLookat.x = 0.0f;
	sgLookat.y = 0.0f;
	sgLookat.z = 0.0f;
}

void moveCamera(float interval, Vector3 camera, Vector3 lookat) {
	Vector3 diff;
	Vector3 up = { 0.0f, 0.0f, 1.0f };
	float error;

	/* new values */
	diff = sub(camera, sgCamera);
	error = len(diff);
	sgCamera = add(sgCamera, scale(5.0f * interval * error, norm(diff)));

	diff = sub(lookat, sgLookat);
	error = len(diff);
	sgLookat = add(sgLookat, scale(5.0f * interval * error, norm(diff)));

	/* set camera */
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	gluLookAt(
			sgCamera.x,  sgCamera.y, sgCamera.z,
			sgLookat.x, sgLookat.y, sgLookat.z,
			up.x, up.y, up.z
			);

	glGetFloatv(GL_MODELVIEW_MATRIX, &sgWindowViewport.view[0][0]);
}
