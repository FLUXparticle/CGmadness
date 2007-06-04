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

#include "water.h"

#include "objects.h"
#include "common.h"
#include "field.h"
#include "texture.h"
#include "environment.h"

#include <GL/gl.h>

#define SHOW_LINES 0

static int gWaterTexture;

void initWater(void) {
	gWaterTexture = loadTexture("data/water.tga", 1);
}

void drawWater(void) {
	int i;

	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, gWaterTexture);
	glDisable(GL_LIGHTING);

#if (SHOW_LINES)
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
#endif

	glPushMatrix();
		glPushMatrix();
			glColor3f(1.0f, 1.0f, 1.0f);

			glTranslatef(0.0f, 0.0f, WATER_LEVEL);

			glNormal3f(0.0f,  0.0f, 1.0f);
			for (i = 0; i < ENVIRONMENT_SIZE; i++) {
				glBegin(GL_TRIANGLE_STRIP);
					
					glTexCoord2f(-i, i);
					glVertex2f(-i, -i);
					glTexCoord2f(-(i + 1), i + 1);
					glVertex2f(-(i + 1), -(i + 1));
					
					glTexCoord2f(i, i);
					glVertex2f(i, -i);
					glTexCoord2f(i + 1, i + 1);
					glVertex2f(i + 1, -(i + 1));
					
					glTexCoord2f(i, -i);
					glVertex2f(i, i);
					glTexCoord2f(i + 1, -(i + 1));
					glVertex2f(i + 1, i + 1);
					
					glTexCoord2f(-i, -i);
					glVertex2f(-i, i);
					glTexCoord2f(-(i + 1), -(i + 1));
					glVertex2f(-(i + 1), i + 1);

					glTexCoord2f(-i, i);
					glVertex2f(-i, -i);
					glTexCoord2f(-(i + 1), i + 1);
					glVertex2f(-(i + 1), -(i + 1));
					
				glEnd();
			}
		glPopMatrix();

	glPopMatrix();

#if (SHOW_LINES)
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
#endif

	glEnable(GL_LIGHTING);
	glDisable(GL_TEXTURE_2D);
}
