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

#include "objects.hpp"

#include GL_H

void drawSquare()
{
	glBegin(GL_POLYGON);
	{
		glNormal3f(0.0f, 0.0f, 1.0f);

		glTexCoord2f(0.0f, 1.0f);
		glVertex2f(-1.0f, -1.0f);

		glTexCoord2f(1.0f, 1.0f);
		glVertex2f(1.0f, -1.0f);

		glTexCoord2f(1.0f, 0.0f);
		glVertex2f(1.0f, 1.0f);

		glTexCoord2f(0.0f, 0.0f);
		glVertex2f(-1.0f, 1.0f);
	}
	glEnd();
}

void drawPanel(float width, float height)
{
	glPushMatrix();
	{
		glTranslatef(width / 2.0f, height / 2.0f, -0.1f);

		glScalef(width / 2.0f, height / 2.0f, 1.0f);

		glColor4f(0.0f, 0.0f, 0.0f, 0.5);

		glEnable(GL_BLEND);
		{
			drawSquare();
		}
		glDisable(GL_BLEND);
	}
	glPopMatrix();
}
