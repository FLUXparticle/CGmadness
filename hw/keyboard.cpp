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

#include "keyboard.hpp"
#include "macros.hpp"

#include GLUT_H

#define MAX_KEYS 256

#define MAX_CURSORS LENGTH(gCursors)

#define MAX_FUNCTION LENGTH(gFunctions)

int gCursors[] = {
	GLUT_KEY_UP,
	GLUT_KEY_DOWN,
	GLUT_KEY_LEFT,
	GLUT_KEY_RIGHT
};

int gFunctions[] = {
	GLUT_KEY_F1,
	GLUT_KEY_F2,
	GLUT_KEY_F3,
	GLUT_KEY_F4,
	GLUT_KEY_F5,
	GLUT_KEY_F6,
	GLUT_KEY_F7,
	GLUT_KEY_F8,
	GLUT_KEY_F9,
	GLUT_KEY_F10,
	GLUT_KEY_F11,
	GLUT_KEY_F12
};

bool gKeys[MAX_KEYS];

bool gCursorKeys[MAX_CURSORS];

bool gFunctionKeys[MAX_FUNCTION];

int gModifiers = 0;

unsigned char gLastChar;

int getModifiers(void)
{
	return gModifiers;
}

unsigned char getLastChar()
{
	return gLastChar;
}

bool isPressed(const bool *key)
{
	return *key == GLUT_DOWN;
}

bool wasPressed(bool *key)
{
	if (isPressed(key))
	{
		*key = GLUT_UP;
		return 1;
	}

	return 0;
}

bool isKeyPressed(unsigned char key)
{
	return isPressed(&gKeys[key]);
}

bool wasKeyPressed(unsigned char key)
{
	return wasPressed(&gKeys[key]);
}

bool isCursorPressed(unsigned int cursor)
{
	return cursor < MAX_CURSORS && isPressed(&gCursorKeys[cursor]);
}

bool wasCursorPressed(unsigned int cursor)
{
	return cursor < MAX_CURSORS && wasPressed(&gCursorKeys[cursor]);
}

bool isFunctionPressed(unsigned int index)
{
	return index >= 1 && index <= MAX_FUNCTION
		&& isPressed(&gFunctionKeys[index - 1]);
}

bool wasFunctionPressed(unsigned int index)
{
	return index >= 1 && index <= MAX_FUNCTION
		&& wasPressed(&gFunctionKeys[index - 1]);
}

void keyboardDown(unsigned char key, int x, int y)
{
	gKeys[key] = GLUT_DOWN;
	gModifiers = glutGetModifiers();

	gLastChar = key;
}

void keyboardUp(unsigned char key, int x, int y)
{
	gKeys[key] = GLUT_UP;
	gModifiers = glutGetModifiers();
}

bool *getSpecialKeyPtr(int key)
{
	for (unsigned int i = 0; i < MAX_CURSORS; i++)
	{
		if (key == gCursors[i])
		{
			return &gCursorKeys[i];
		}
	}

	for (unsigned int i = 0; i < MAX_FUNCTION; i++)
	{
		if (key == gFunctions[i])
		{
			return &gFunctionKeys[i];
		}
	}

	return NULL;
}

void specialKeyDown(int key, int x, int y)
{
	bool *ptr = getSpecialKeyPtr(key);

	if (ptr)
	{
		*ptr = GLUT_DOWN;
		gModifiers = glutGetModifiers();
	}
}

void specialKeyUp(int key, int x, int y)
{
	bool *ptr = getSpecialKeyPtr(key);

	if (ptr)
	{
		*ptr = GLUT_UP;
		gModifiers = glutGetModifiers();
	}
}

void startKeyboard(void)
{
	for (int i = 0; i < MAX_KEYS; i++)
	{
		gKeys[i] = GLUT_UP;
	}
	for (unsigned int i = 0; i < MAX_CURSORS; i++)
	{
		gCursorKeys[i] = GLUT_UP;
	}
	for (unsigned int i = 0; i < MAX_FUNCTION; i++)
	{
		gFunctionKeys[i] = GLUT_UP;
	}

	glutKeyboardFunc(keyboardDown);
	glutKeyboardUpFunc(keyboardUp);
	glutSpecialFunc(specialKeyDown);
	glutSpecialUpFunc(specialKeyUp);
}
