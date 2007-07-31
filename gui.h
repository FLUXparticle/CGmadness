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

#ifndef _gui_h_
#define _gui_h_

#include "graph.h"
#include "pick.h"
#include "vector.h"
#include "types.h"

typedef void (*funcClick)(void);

typedef void (*funcChange)(void* self);

typedef struct {
	int value;
	Object oCheck;
	Pick pCheck;
	funcChange change;
} Check;

typedef struct {
	int value;
	int minValue;
	int maxValue;
	Object oSpinEdit;
	Object oLeft;
	Object oRight;
	Pick pLeft;
	Pick pRight;
	funcChange change;
} SpinEdit;

/***/

typedef enum {
	MI_UNDEFINED = -1,
	MI_BUTTON
} MenuItemType;

typedef struct {
	MenuItemType type;

	Vector2 position;
	float width;
	float height;

	Pick pick;
	int pickName;
} MenuItem;

typedef struct {
	int cntItems;
	MenuItem** items;
} Menu;

typedef struct {
	MenuItem item;

	char* text;
	funcClick click;
} Button;

void initGUI(void);

void setSomeLight(void);

/* Button */

void initButton(Button* button, float z, funcClick click, char* text);

/* Check */

void setCheck(Check* check, int value);
void init3dCheck(Check* check, float z, funcChange change, char* text);

/* SpinEdit */

void init3dSpinEdit(SpinEdit* spinedit, int value, int min, int max, float z, Object* obj, funcChange change);

/* MenuItem */

void drawMenuItem(const MenuItem* item);
void pickMenuItem(MenuItem* item);
void clickMenuItem(MenuItem* item);

/* Menu */

#define INIT_MENU(menu, items) initMenu((menu), LENGTH(items), (items))

void initMenu(Menu* menu, int cntItems, MenuItem** items);
void drawMenu(const Menu* menu);

#endif
