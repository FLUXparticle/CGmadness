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

#include "skysphere.h"

#include "environment.h"

#include "vector.hpp"
#include "types.hpp"

#include <stdio.h>

#define DAMPENING 0.8

#define SIDES  12
#define SLICES 3

/* Vertexcount of the dome */
#define NUMVERTS ((SLICES + 1) * (SIDES + 1))
#define NUMINDICES (SLICES * (SIDES + 1) * 2)

#define SKY_TEXTURE 0

static Vector3 gVertexBuffer[NUMVERTS];
#if (SKY_TEXTURE)
static Vector2 gTextureBuffer[NUMVERTS];
#endif
static GLuint gIndexBuffer[NUMINDICES];

void initSkysphere(void)
{
	int i;
	int j;
	int ind;
	float vx, vy, vz;

	/* Calculate deltaTheta */
	float angle1;
	float angle2;

	for (j = 0; j <= SLICES; j++)
	{
		/* deltaPhi */
		angle1 = j * (PI / 2.0f) / SLICES;

		for (i = 0; i <= SIDES; i++)
		{
			angle2 = i * (2.0f * PI) / SIDES;

			/* Calculate Vertexpositions */
			vx = cos(angle2) * cos(angle1);
			vy = sin(angle2) * cos(angle1);
			vz = 0.1f * sin(angle1);

			gVertexBuffer[j * (SIDES + 1) + i].x = vx;
			gVertexBuffer[j * (SIDES + 1) + i].z = vz;
			gVertexBuffer[j * (SIDES + 1) + i].y = vy;

#if (SKY_TEXTURE)
			gTextureBuffer[j * (SIDES + 1) + i].x = (float) i / SIDES;
			gTextureBuffer[j * (SIDES + 1) + i].y = (float) j / SLICES;
#endif
		}
	}

	/* Calculate Indices */
	ind = 0;
	for (j = 1; j <= SLICES; j++)
	{
		for (i = 0; i <= SIDES; i++)
		{
			gIndexBuffer[ind++] = (j - 1) * (SIDES + 1) + i;
			gIndexBuffer[ind++] = j * (SIDES + 1) + i;
		}
	}
}

void drawSkysphere(void)
{
	int j;

	glColor3f(0.651f, 0.682f, 1.0f);

	glPushMatrix();

	glScalef(ENVIRONMENT_SIZE, ENVIRONMENT_SIZE, ENVIRONMENT_SIZE);

	glEnableClientState(GL_VERTEX_ARRAY);
	glVertexPointer(3, GL_FLOAT, 0, gVertexBuffer);

	for (j = 0; j < SLICES; j++)
	{
		glDrawElements(GL_TRIANGLE_STRIP, (SIDES + 1) * 2, GL_UNSIGNED_INT,
									 &gIndexBuffer[j * (SIDES + 1) * 2]);
	}

	glDisableClientState(GL_VERTEX_ARRAY);

	glPopMatrix();
}
