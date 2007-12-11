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

#include "text.h"

#include "dataBigAlpha.h"
#include "dataSmallAlpha.h"
#include "dataDigits.h"


#include "debug.h"

#include <GL/glut.h>

#include <stdio.h>

#define SCALE 0.1f

#define STROKE_SIZE 119.05f

void drawBitmapText(const char *str)
{
	for (; *str; str++) {
		glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, *str);
	}
}

float widthStrokeText(const char* str)
{
	float width = 0.0f;

	const char* s;

  for (s = str; *s; s++) {
		width += glutStrokeWidth(GLUT_STROKE_ROMAN, *s);
	}

	return width / STROKE_SIZE;
}

static void drawStrokeText(const char* str)
{
	float scale = 1.0f / STROKE_SIZE;

	const char* s;

	glPushMatrix();

		glScalef(scale, scale, scale);

			for (s = str; *s; s++) {
				glutStrokeCharacter(GLUT_STROKE_ROMAN, *s);
			}

			glLineWidth(1.0f);
			
		glDisable(GL_BLEND);
		glDisable(GL_LINE_SMOOTH);

	glPopMatrix();
}

void drawStrokeThinText(const char* str)
{
	glLineWidth(1.5f);
	
	drawStrokeText(str);

	glLineWidth(1.0f);
}

void drawStrokeThickText(const char* str)
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

float widthFont3DText(const char* str)
{
	float width = 0.0f;

	const char* s;

	for (s = str; *s; s++) {
		if (*s >= 'A' && *s <= 'Z') {
			int i = *s - 'A';
			width += widthBigAlpha[i];
		}
		if (*s >= 'a' && *s <= 'z') {
			int i = *s - 'a';
			width += widthSmallAlpha[i];
		}
		if (*s >= '0' && *s <= '9') {
			int i = *s - '0';
			width += widthDigits[i];
		}
		if (*s == ' ') {
			width += 5.0f;
		}
	}

	return width;
}

void drawFont3DChar(funcDraw draw, float width)
{
	draw();
	glTranslatef(width, 0.0f, 0.0f);
}

void drawFont3DText(const char* str)
{
	const char* s;

	for (s = str; *s; s++) {
		if (*s >= 'A' && *s <= 'Z') {
			int i = *s - 'A';
			drawFont3DChar(drawBigAlpha[i], widthBigAlpha[i]);
		}
		if (*s >= 'a' && *s <= 'z') {
			int i = *s - 'a';
			drawFont3DChar(drawSmallAlpha[i], widthSmallAlpha[i]);
		}
		if (*s >= '0' && *s <= '9') {
			int i = *s - '0';
			drawFont3DChar(drawDigits[i], widthDigits[i]);
		}
		if (*s == ' ') {
			glTranslatef(5.0f, 0.0f, 0.0f);
		}
	}
}
