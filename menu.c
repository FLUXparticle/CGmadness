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

#include "menu.h"

#include "texture.h"
#include "objects.h"

#include <GL/gl.h>

static GLuint gTexLogo = 0;

void initLogo(void)
{
	if (gTexLogo == 0)
	{
		gTexLogo = loadTexture("data/logo.tga", 0);
	}
}

void drawLogo(void)
{

	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, gTexLogo);

		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

			glPushMatrix();

				glTranslatef(0.0f, 8.0f, 0.0f);
				glScalef(4.0f, 1.0f, 1.0f);

				drawSquare();

			glPopMatrix();

		glDisable(GL_BLEND);

	glDisable(GL_TEXTURE_2D);
}
