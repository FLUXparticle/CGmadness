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

#include "text.hpp"

#include GLUT_H

#define SCALE 0.1f

#define STROKE_SIZE 119.05f

void drawBitmapText(const char *str)
{
	for (; *str; str++)
	{
		glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, *str);
	}
}

float widthStrokeText(const char *str)
{
	float width = 0.0f;

	const char *s;

	for (s = str; *s; s++)
	{
		width += glutStrokeWidth(GLUT_STROKE_ROMAN, *s);
	}

	return width / STROKE_SIZE;
}

static void drawStrokeText(const char *str)
{
	float scale = 1.0f / STROKE_SIZE;

	const char *s;

	glPushMatrix();

	glScalef(scale, scale, scale);

	for (s = str; *s; s++)
	{
		glutStrokeCharacter(GLUT_STROKE_ROMAN, *s);
	}

	glLineWidth(1.0f);

	glDisable(GL_BLEND);
	glDisable(GL_LINE_SMOOTH);

	glPopMatrix();
}

void drawStrokeThinText(const char *str)
{
	glLineWidth(1.5f);

	drawStrokeText(str);

	glLineWidth(1.0f);
}

void drawStrokeThickText(const char *str)
{
	glEnable(GL_BLEND);
	glEnable(GL_LINE_SMOOTH);

	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glLineWidth(3.5f);

	drawStrokeText(str);

	glLineWidth(1.0f);

	glDisable(GL_BLEND);
	glDisable(GL_LINE_SMOOTH);

	glPopMatrix();
}
