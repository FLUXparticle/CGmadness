/*
 * CG Madness - a Marble Madness clone
 * Copyright (C) 2007  Sven Reinck <sreinck@gmx.de>
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

#include "water.h"

#include "texture.h"
#include "environment.h"

#include <GL/glew.h>

#include <stdio.h>

static int gWaterTexture;
static float gWaterAnim;

void initWater(void)
{
	gWaterTexture = loadTexture("data/water.tga", 1);
	gWaterAnim = 0.0f;
}

void updateWater(float interval)
{
	gWaterAnim += interval * 0.05f;

	if (gWaterAnim > 2.0f) {
		gWaterAnim -= 2.0f;
	}
}

void multiTexCoord2f(float u, float v)
{
	glMultiTexCoord2f(GL_TEXTURE0, 1.0f * gWaterAnim + 0.25f * u, 1.0f * gWaterAnim + 0.25f * v + 0.33f);
	glMultiTexCoord2f(GL_TEXTURE1, 0.5f * gWaterAnim + 0.25f * u, 0.5f * gWaterAnim + 0.25f * v);
}

void drawWaterPolygones(void)
{
	float i = 0.0f;
	float j;

	for (j = 1.0f; i < ENVIRONMENT_SIZE; j *= 2)
	{
		glBegin(GL_TRIANGLE_STRIP);

			multiTexCoord2f(-i, i);
			glVertex2f(-i, -i);
			multiTexCoord2f(-j, j);
			glVertex2f(-j, -j);

			multiTexCoord2f(i, i);
			glVertex2f(i, -i);
			multiTexCoord2f(j, j);
			glVertex2f(j, -j);

			multiTexCoord2f(i, -i);
			glVertex2f(i, i);
			multiTexCoord2f(j, -j);
			glVertex2f(j, j);

			multiTexCoord2f(-i, -i);
			glVertex2f(-i, i);
			multiTexCoord2f(-j, -j);
			glVertex2f(-j, j);

			multiTexCoord2f(-i, i);
			glVertex2f(-i, -i);
			multiTexCoord2f(-j, j);
			glVertex2f(-j, -j);

		glEnd();

		i = j;
	}
}

void drawWater(void)
{
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		glNormal3f(0.0f, 0.0f, 1.0f);

		glColor4f(1.0f, 1.0f, 1.0f, 0.8f);

		glActiveTexture(GL_TEXTURE0);
		glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, gWaterTexture);

		glActiveTexture(GL_TEXTURE1);
		glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, gWaterTexture);

			drawWaterPolygones();

		glActiveTexture(GL_TEXTURE1);
		glDisable(GL_TEXTURE_2D);
		glActiveTexture(GL_TEXTURE0);
		glDisable(GL_TEXTURE_2D);

	glDisable(GL_BLEND);
}
