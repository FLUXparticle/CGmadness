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

#include "text.h"

#include "dataBigAlpha.h"
#include "dataSmallAlpha.h"
#include "dataDigits.h"

#include "graph.h"

#include "debug.h"

#include <GL/glut.h>

#define SCALE 0.1f

void drawBitmapText(const char *str) {
	glDisable(GL_DEPTH_TEST);
	for (; *str; str++) {
		glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, *str);
	}
	glEnable(GL_DEPTH_TEST);
}

void addChar(Object* obj, float* x, funcDraw draw, float width) {
	Object* oChar;

	MALLOC(oChar, sizeof(Object));

	initObject(oChar, draw);

	setObjectScalef(oChar, SCALE);

	oChar->pos.x = *x;
	*x += width * SCALE;

	addSubObject(obj, oChar);
}

float makeTextObject(Object* obj, const char* text) {
	const char* s;
	float x = 0;
	
	initObjectGroup(obj);

	for (s = text; *s; s++) {
		if (*s >= 'A' && *s <= 'Z') {
			int i = *s - 'A';
			addChar(obj, &x, drawBigAlpha[i], widthBigAlpha[i]);
		}
		if (*s >= 'a' && *s <= 'z') {
			int i = *s - 'a';
			addChar(obj, &x, drawSmallAlpha[i], widthSmallAlpha[i]);
		}
		if (*s >= '0' && *s <= '9') {
			int i = *s - '0';
			addChar(obj, &x, drawDigits[i], widthDigits[i]);
		}
		if (*s == ' ') {
			x += 0.5f;
		}
	}

	return x;
}
