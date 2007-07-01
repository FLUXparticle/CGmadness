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

#include "functions.h"

#include <GL/gl.h>

#include <stdio.h>

static int gWaterTexture;
static float gWaterAnim;

void initWater(void) {
	gWaterTexture = loadTexture("data/water.tga", 1);
	gWaterAnim = 0.0f;
}

void updateWater(float interval) {
	gWaterAnim += interval * 0.05f;

	if (gWaterAnim > 2.0f) {
		gWaterAnim -= 2.0f;
	}
}

void drawWaterPolygones(void) {
	float i = 0.0f;
	float j;

	for (j = 1.0f; i < ENVIRONMENT_SIZE; j *= 2) {
		glBegin(GL_TRIANGLE_STRIP);

			glTexCoord2f(-i, i);
			glVertex2f(-i, -i);
			glTexCoord2f(-j, j);
			glVertex2f(-j, -j);

			glTexCoord2f(i, i);
			glVertex2f(i, -i);
			glTexCoord2f(j, j);
			glVertex2f(j, -j);

			glTexCoord2f(i, -i);
			glVertex2f(i, i);
			glTexCoord2f(j, -j);
			glVertex2f(j, j);

			glTexCoord2f(-i, -i);
			glVertex2f(-i, i);
			glTexCoord2f(-j, -j);
			glVertex2f(-j, j);

			glTexCoord2f(-i, i);
			glVertex2f(-i, -i);
			glTexCoord2f(-j, j);
			glVertex2f(-j, -j);

		glEnd();

		i = j;
	}
}

void drawWater(void) {
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, gWaterTexture);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glNormal3f(0.0f, 0.0f, 1.0f);

	glColor4f(1.0f, 1.0f, 1.0f, 0.7f);

	glPushMatrix();

		glTranslatef(0.0f, 0.0f, WATER_LEVEL);

		glMatrixMode(GL_TEXTURE);
		glPushMatrix();
			glTranslatef(gWaterAnim, gWaterAnim, 0.0f);
			glScalef(0.25f, 0.25f, 1.0f);

			drawWaterPolygones();

		glPopMatrix();
		glMatrixMode(GL_MODELVIEW);

	glPopMatrix();

	glDisable(GL_DEPTH_TEST);

		glColor4f(1.0f, 1.0f, 1.0f, 0.5f);

		glPushMatrix();

			glTranslatef(0.0f, 0.0f, WATER_LEVEL);

			glMatrixMode(GL_TEXTURE);
			glPushMatrix();
				glTranslatef(gWaterAnim * 0.5f, gWaterAnim * 0.5f, 0.0f);
				glScalef(0.25f, 0.25f, 1.0f);

				drawWaterPolygones();

			glPopMatrix();
			glMatrixMode(GL_MODELVIEW);

		glPopMatrix();
	glEnable(GL_DEPTH_TEST);

	glDisable(GL_BLEND);

	glDisable(GL_TEXTURE_2D);
}
