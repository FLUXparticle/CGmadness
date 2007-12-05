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

#ifndef _keyboard_h_
#define _keyboard_h_

#define KEY_ESC 27
#define KEY_ENTER 13

#define CURSOR_UP 0
#define CURSOR_DOWN 1
#define CURSOR_LEFT 2
#define CURSOR_RIGHT 3

int getModifiers(void);
unsigned char getLastChar(void);

int wasKeyPressed(unsigned char key);
int isKeyPressed(unsigned char key);
int isCursorPressed(int cursor);
int wasCursorPressed(int cursor);
int isFunctionPressed(int index);
int wasFunctionPressed(int index);

void startKeyboard(void);

#endif
