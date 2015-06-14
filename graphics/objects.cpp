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

#include "graphics/objects.hpp"

#include "ColorStack.hpp"

#include "functions.hpp"

#include <cmath>

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

void drawCube()
{
	glPushMatrix();
	{
		for (int i = 0; i < 6; ++i)
		{
			glRotatef(90.0f, 1 - i % 2, i % 2, 0.0f);
			glPushMatrix();
			{
				glTranslatef(0.0f, 0.0f, 1.0f);
				drawSquare();
			}
			glPopMatrix();
		}
		glPolygonMode(GL_FRONT, GL_FILL);
	}
	glPopMatrix();
}

void drawPanel(float width, float height)
{
	glPushMatrix();
	{
		glTranslatef(width / 2.0f, height / 2.0f, -0.1f);
		glScalef(width / 2.0f, height / 2.0f, 1.0f);

		ColorStack::colorStack.setColor(Color4(0.0f, 0.0f, 0.0f, 0.5f));

		drawSquare();
	}
	glPopMatrix();
}

void drawRingStrip(int corners, float progress, int texture)
{
	static float step = 0.05f / (2.0f * M_PI);
	static float f = 1.0f - 2.0f * step;

	float p = 2.0f * f * progress;

	float p0 = max(0.0f, p - f);
	float p2 = min(1.0f - step, p + step);

	float p1 = p0 + step;
	float p3 = p2 + step;

	float diff = p2 - p0;

	int steps = (int) (diff / step + 0.5f);

	glColor3f(1.0f, 1.0f, 0.0f);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	{
		glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, texture);
		{
			glBegin(GL_TRIANGLE_STRIP);
			{
				float angle = 2.0f * M_PI * p0;

				glTexCoord2f(0.0f, 0.0f);
				glVertex2f(0.9f * cos(angle), 0.9f * sin(angle));

				glTexCoord2f(0.0f, 1.0f);
				glVertex2f(1.0f * cos(angle), 1.0f * sin(angle));

				for (int i = 0; i <= steps; i++)
				{
					float t = (float) i / steps;
					angle = 2.0f * M_PI * ((1.0f - t) * p1 + t * p2);

					glTexCoord2f(0.5f, 0.0f);
					glVertex2f(0.9f * cos(angle), 0.9f * sin(angle));

					glTexCoord2f(0.5f, 1.0f);
					glVertex2f(1.0f * cos(angle), 1.0f * sin(angle));
				}

				angle = 2.0f * M_PI * p3;

				glTexCoord2f(1.0f, 0.0f);
				glVertex2f(0.9f * cos(angle), 0.9f * sin(angle));

				glTexCoord2f(1.0f, 1.0f);
				glVertex2f(1.0f * cos(angle), 1.0f * sin(angle));
			}
			glEnd();
		}
		glDisable(GL_TEXTURE_2D);
	}
	glDisable(GL_BLEND);
}

