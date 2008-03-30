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

#include "mouse.h"

#include "callback.h"

#include <GL/glut.h>


#define SELECT_BUFFER_SIZE 512

static GLuint gSelectBuffer[SELECT_BUFFER_SIZE];

static int gLastX;
static int gLastY;
static int gButton;

static funcDrag gDrag;

typedef struct
{
	GLuint stackSize;
	GLuint minDepth;
	GLuint maxDepth;
	GLuint stackBottom;
} SelectBuffer;

void mouseButton(int button, int state, int x, int y)
{
	gButton = button;
	gLastX = x;
	gLastY = y;

	if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
	{
		mouseEvent(x, y, MOUSE_CLICK);
	}
}

void mouseMotion(int x, int y)
{
	if (x != gLastX || y != gLastY)
	{
		if (gDrag)
		{
			gDrag(x - gLastX, y - gLastY);
			glutWarpPointer(gLastX, gLastY);
		}
		else
		{
			mouseEvent(x, y, MOUSE_MOTION);
		}
	}
}

void setDragFunc(funcDrag drag)
{
	gDrag = drag;
	if (gDrag)
	{
		centerMouse(&gLastX, &gLastY);
	}
	else
	{
		mouseEvent(gLastX, gLastY, MOUSE_MOTION);
	}
}

void startMouse(void)
{
	glSelectBuffer(SELECT_BUFFER_SIZE, gSelectBuffer);
	glutMouseFunc(mouseButton);
	glutPassiveMotionFunc(mouseMotion);
}
