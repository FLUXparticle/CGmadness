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

#include "keyboard.h"
#include "types.h"

#include <GL/glut.h>

#define MAX_KEYS 256

#define MAX_CURSORS length(gCursors)

#define MAX_FUNCTION length(gFunctions)

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

int gKeys[MAX_KEYS];

int gCursorKeys[MAX_CURSORS];

int gFunctionKeys[MAX_FUNCTION];

/*
 * Funktionen zum Abfragen der Tasten-Zustände
 */
int isPressed(int* key) {
	return *key == GLUT_DOWN;
}

/*
 * Gibt zurück, ob eine Taste gedrückt wurde und löscht den Tastendruck aus dem Speicher,
 * damit nur einmal darauf reagiert wird.
 */
int wasPressed(int* key) {
	if (isPressed(key)) {
		*key = GLUT_UP;
		return 1;
	}

	return 0;
}

int isKeyPressed(unsigned char key) {
	return isPressed(&gKeys[key]);
}

int wasKeyPressed(unsigned char key) {
	return wasPressed(&gKeys[key]);
}

int isCursorPressed(int cursor) {
	return cursor >= 0 && cursor < MAX_CURSORS && isPressed(&gCursorKeys[cursor]);
}

int wasCursorPressed(int cursor) {
	return cursor >= 0 && cursor < MAX_CURSORS && wasPressed(&gCursorKeys[cursor]);
}

int isFunctionPressed(int index) {
	return index >= 1 && index < MAX_FUNCTION && isPressed(&gFunctionKeys[index - 1]);
}

int wasFunctionPressed(int index) {
	return index >= 1 && index < MAX_FUNCTION && wasPressed(&gFunctionKeys[index - 1]);
}

/*
 * Callback-Funktionen fÃ¼r den Keyboard-Handler
 */
void keyboardDown(unsigned char key, int x, int y) {
	gKeys[key] = GLUT_DOWN;
}

void keyboardUp(unsigned char key, int x, int y) {
	gKeys[key] = GLUT_UP;
}

int* getSpecialKeyPtr(int key) {
	int i;
	for (i = 0; i < MAX_CURSORS; i++) {
		if (key == gCursors[i]) {
			return &gCursorKeys[i];
		}
	}

	for (i = 0; i < MAX_FUNCTION; i++) {
		if (key == gFunctions[i]) {
			return &gFunctionKeys[i];
		}
	}

	return NULL;
}

void specialKeyDown(int key, int x, int y) {
	int* ptr = getSpecialKeyPtr(key);

	if (ptr) {
		*ptr = GLUT_DOWN;
	}
}

void specialKeyUp(int key, int x, int y) {
	int* ptr = getSpecialKeyPtr(key);

	if (ptr) {
		*ptr = GLUT_UP;
	}
}

/*
 * Richte Keyboard-Handler ein
 */
void startKeyboard(void) {
	int i;
	for (i = 0; i < MAX_KEYS; i++) {
		gKeys[i] = GLUT_UP;
	}
	for (i = 0; i < MAX_CURSORS; i++) {
		gCursorKeys[i] = GLUT_UP;
	}
	for (i = 0; i < MAX_FUNCTION; i++) {
		gFunctionKeys[i] = GLUT_UP;
	}
	
	/* Tasten-Druck-Callback (wird ausfuehrt, wenn eine Taste gedrueckt wird) */
	glutKeyboardFunc(keyboardDown);

	/* Tasten-Loslass-Callback (wird ausfuehrt, wenn eine Taste
	 * los gelassen wird) */
	glutKeyboardUpFunc(keyboardUp);

	/* Spezialtasten-Druck-Callback (wird ausfuehrt, wenn Spezialtaste
	 * (F1 - F12, Links, Rechts, Oben, Unten, Bild-Auf, Bild-Ab, Pos1,
	 * Ende oder Einfuegen ) gedrueckt wird) */
	glutSpecialFunc(specialKeyDown);

	/* Spezialtasten-Loslass-Callback (wird ausfuehrt, wenn eine
	 * Spezialtaste los gelassen wird) */
	glutSpecialUpFunc(specialKeyUp);
}
