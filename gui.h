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

#ifndef _gui_h_
#define _gui_h_

#include "vector.h"
#include "mouse.h"
#include "types.h"

typedef void (*funcClick)(void);

typedef void (*funcChange)(const void* self);

/***/

typedef enum {
	MI_CANVAS,
	MI_LABEL,
	MI_PROGRESS_BAR,
	MI_BUTTON,
	MI_CHECK,
	MI_SPIN_EDIT
} MenuItemType;

typedef struct {
	MenuItemType type;

	Vector2 position;
	float width;
	float height;

	int hover;
	float emphasize;
} MenuItem;

typedef struct Screen {
	int cntItems;
	MenuItem** items;

	struct Screen* back;
} Screen;

typedef struct {
	MenuItem item;
	
	funcUpdate customUpdate;
	funcDraw customDraw;
} Canvas;

typedef struct {
	MenuItem item;

	char* text;
} Label;

typedef struct {
	MenuItem item;

	float* progress;
} ProgressBar;

typedef struct {
	MenuItem item;

	char* text;
	int shortcut;

	funcClick click;
} Button;

typedef struct {
	MenuItem item;

	char* text;
	int value;

	funcChange change;
} Check;

typedef struct {
	MenuItem item;

	int value;
	int minValue;
	int maxValue;

	int side;

	funcDraw draw;
	funcChange change;
} SpinEdit;

void initGUI(void);

void setSomeLight(void);

/* Canvas */

void initCanvas(Canvas* canvas, float z, float width, float height, funcUpdate customUpdate, funcDraw customDraw);

/* Label */

void initLabel(Label* label, float x, float z, int alignRight, char* text);

/* ProgressBar */

void initProgressBar(ProgressBar* progressBar, float z, float* progress);

/* Button */

void initButton(Button* button, float z, funcClick click, char* text, int shortcut);

/* Check */

void setCheck(Check* check, int value);
void initCheck(Check* check, float z, funcChange change, char* text);

/* SpinEdit */

void initSpinEdit(SpinEdit* spinedit, int value, int min, int max, float width, float z, funcDraw draw, funcChange change);

/* Screen */

#define INIT_SCREEN(screen, items) initScreen((screen), LENGTH(items), (items))

void initScreen(Screen* screen, int cntItems, MenuItem** items);
void prepareScreen(Screen* screen);
void updateScreen(Screen* screen, float interval);
void drawScreen(const Screen* screen);
void eventScreen(Screen* screen, float x, float y, MouseEvent event);

#endif
