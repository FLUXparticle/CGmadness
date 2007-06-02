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

static int gWaterTexture;

void initWater(void) {
	gWaterTexture = loadTexture("data/water.tga", 1);
}

void drawWater(void) {
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, gWaterTexture);
	glDisable(GL_LIGHTING);

	glPushMatrix();
		glPushMatrix();
			glColor3f(1.0f, 1.0f, 1.0f);

			glTranslatef(0.0f, 0.0f, WATER_LEVEL);
			glScalef(ENVIRONMENT_SIZE, ENVIRONMENT_SIZE, 1.0f);

			glBegin(GL_POLYGON);
				glNormal3f(0.0f,  0.0f, 1.0f);
				
				glTexCoord2f(ENVIRONMENT_SIZE, ENVIRONMENT_SIZE);
				glVertex2f(-1.0f, -1.0f);
				
				glTexCoord2f(-ENVIRONMENT_SIZE, ENVIRONMENT_SIZE);
				glVertex2f(1.0f, -1.0f);
				
				glTexCoord2f(-ENVIRONMENT_SIZE, -ENVIRONMENT_SIZE);
				glVertex2f(1.0f, 1.0f);
				
				glTexCoord2f(ENVIRONMENT_SIZE, -ENVIRONMENT_SIZE);
				glVertex2f(-1.0f, 1.0f);
			glEnd();
		glPopMatrix();

	glPopMatrix();

	glEnable(GL_LIGHTING);
	glDisable(GL_TEXTURE_2D);
}
