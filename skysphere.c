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

#include "skysphere.h"

#include "environment.h"

#include "vector.h"
#include "types.h"

#include <stdio.h>

#define DAMPENING 0.8

/* SIDES of the skydome */
#define SIDES  30

/* SLICES of the skydome */
#define SLICES 48

/* Vertexcount of the dome */
#define NUMVERTS ((SLICES + 1) * (SIDES + 1))
#define NUMINDICES (SLICES * (SIDES + 1) * 2)

#define USE_VERTEX_ARRAY 0

Vector3 VertexBuffer[NUMVERTS];
Vector2 TextureBuffer[NUMVERTS];
GLuint IndexBuffer[NUMINDICES];

void initSkysphere(void) {
	int i;  
	int j;
	int ind;
	float vx,vy,vz;
	
	/* Calculate deltaTheta */
	float angle1;
	float angle2;
	
	for (j = 0; j <= SLICES; j++) {
		/* deltaPhi */
		angle1 = j * (PI / 2.0f) / SLICES;
		
		for (i = 0; i <= SIDES; i++) {
			angle2 = i * (2.0f * PI) / SIDES;

			/* Calculate Vertexpositions */
			vx = cos(angle2) * cos(angle1);
			vy = sin(angle2) * cos(angle1);
			vz = 0.1f * sin(angle1);
			
			VertexBuffer[j * (SIDES + 1) + i].x = vx;
			VertexBuffer[j * (SIDES + 1) + i].z = vz;
			VertexBuffer[j * (SIDES + 1) + i].y = vy;
			
#if 0
			TextureBuffer[j * (SIDES + 1) + i].x = (float)(i) / (float)(SIDES);
			TextureBuffer[j * (SIDES + 1) + i].y = (float)(j) / (float)(SLICES);
#endif
		}
	}
	
	/* Calculate Indices */
	ind = 0;
	for(j = 1; j <= SLICES; j++) {
		for(i = 0; i <= SIDES; i++) {
			IndexBuffer[ind++] = (j - 1) * (SIDES + 1) + i;
			IndexBuffer[ind++] = j * (SIDES + 1) + i;
		}
	}	
}

void drawSkysphere(void) {
	int i, j;
	
	glColor3f(0.651f, 0.682f, 1.0f);
	glDisable(GL_LIGHTING);

	glPushMatrix();

		glTranslatef(0.0f, 0.0f, WATER_LEVEL);
		glScalef(ENVIRONMENT_SIZE, ENVIRONMENT_SIZE, ENVIRONMENT_SIZE);

#if (USE_VERTEX_ARRAY)
		glEnableClientState(GL_VERTEX_ARRAY);
		glVertexPointer(3, GL_FLOAT, 0, VertexBuffer);
#endif

		for (j = 0; j < SLICES; j++) {
#if (USE_VERTEX_ARRAY)
			glDrawElements(GL_TRIANGLE_STRIP, (SIDES + 1) * 2, GL_UNSIGNED_INT, &IndexBuffer[j * (SIDES + 1) * 2]);
#else
			glBegin(GL_TRIANGLE_STRIP);
			for (i = 0; i < (SIDES + 1) * 2; i++) {
				glVertex3f(VertexBuffer[IndexBuffer[j * (SIDES + 1) * 2 + i]].x, VertexBuffer[IndexBuffer[j * (SIDES + 1) * 2 + i]].y, VertexBuffer[IndexBuffer[j * (SIDES + 1) * 2 + i]].z);
			}
			glEnd();
#endif
		}

#if (USE_VERTEX_ARRAY)
		glDisableClientState(GL_VERTEX_ARRAY);
#endif
	
	glPopMatrix();

	glEnable(GL_LIGHTING);
}
