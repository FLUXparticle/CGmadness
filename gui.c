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

#include "gui.h"

#include "text.h"
#include "objects.h"
#include "texture.h"
#include "debug.h"

#include <GL/glut.h>

#include <stdio.h>
#include <string.h>

#define SCALE_FONT 0.5f

/*** common ***/

static float scaleButton = 0.1f * SCALE_FONT;

static Object goLeft;
static Object goRight;

static int gInitialize = 0;

void initMenuObject(Object* obj, int texture) {
	initObject(obj, drawSquare);
	obj->texture = texture;
	obj->ambient = 1.0f;
	obj->diffuse = 0.0f;
}

void initTextObject(Object* obj, char* text, float z) {
	float length = makeTextObject(obj, text) * SCALE_FONT;
	setObjectPosition3f(obj, -length / 2, 0.0f, z);
	setObjectScalef(obj, SCALE_FONT);
	rotateObjectX(obj, 90.0f);
}

void initGUI() {
	if (!gInitialize) {
		/* loading arrow textures */
		initMenuObject(&goLeft, loadTexture("data/left.tga", 0));
		initMenuObject(&goRight, loadTexture("data/right.tga", 0));

		gInitialize = 1;
	}
}

void setSomeLight(void) {
	float pos[4]  = { 0.0f, -1.0f, 0.0f, 0.0f };
	float ambient[4]  = { 0.2f, 0.2f, 0.2f, 1.0f };
	float diffuse[4]  = { 1.0f, 1.0f, 1.0f, 1.0f };
	float specular[4] = { 0.0f, 0.0f, 0.0f, 1.0f };

	glLightfv(GL_LIGHT0, GL_POSITION, pos);
	glLightfv(GL_LIGHT0, GL_AMBIENT,  ambient);
	glLightfv(GL_LIGHT0, GL_DIFFUSE,  diffuse);
	glLightfv(GL_LIGHT0, GL_SPECULAR, specular);
	glEnable(GL_LIGHT0);
}

/*** Button ***/

void initButton(Button* button, float z, funcClick click, char* text)
{
	button->click = click;
	button->text = text;

	button->item.width = widthFont3DText(button->text) * scaleButton;
	button->item.height = 0.9f;

	button->item.position = vector2(-button->item.width / 2.0f, z);
}

void clickButton(Button* button)
{
	if (button->click)
	{
		button->click();
	}
}

void drawButton(const Button* button)
{
	float scale = scaleButton * (1.0f + 0.1f * button->item.emphasize);

	glPushMatrix();

		glTranslatef(-0.5f * button->item.width * 0.1f * button->item.emphasize, 0.0f, 0.0f);
		glScalef(scale, scale, scale);
		drawFont3DText(button->text);

	glPopMatrix();
}

/*** Check ***/

void setCheck(Check* check, int value) {
	check->value = value;

#if 0
	if (check->value) {
		setObjectGroupColor(&check->oCheck, 1.0f, 1.0f, 1.0f);
	} else {
		setObjectGroupColor(&check->oCheck, 0.5f, 0.5f, 0.5f);
	}
#endif

	check->change(check);
}

void pickCheck(void* data) {
	Check* check = data;
	setCheck(check, !check->value);
}

void init3dCheck(Check* check, float z, funcChange change, char* text) {
#if 0
	initTextObject(&check->oCheck, text, z);
#endif

	check->change = change;

	setCheck(check, 1);
}

/*** SpinEdit ***/

void pickSpinEditLeft(void* data) {
	SpinEdit* spinedit = data;
	int* value = &spinedit->value;
	if (*value > spinedit->minValue) {
		(*value)--;
		spinedit->change(spinedit);
	}
}

void pickSpinEditRight(void* data) {
	SpinEdit* spinedit = data;
	int* value = &spinedit->value;
	if (*value < spinedit->maxValue) {
		(*value)++;
		spinedit->change(spinedit);
	}
}

void init3dSpinEdit(SpinEdit* spinedit, int value, int min, int max, float z, Object* obj, funcChange change) {
	spinedit->value = value;
	spinedit->minValue = min;
	spinedit->maxValue = max;

#if 0
	initObjectGroup(&spinedit->oSpinEdit);

	setObjectPosition3f(&spinedit->oSpinEdit, 0.0f, 0.0f, z);
	rotateObjectX(&spinedit->oSpinEdit, 90.0f);
	setObjectScalef(&spinedit->oSpinEdit, SCALE_FONT);

	/* arrow left */
	initObjectGroup(&spinedit->oLeft);
	setObjectPosition2f(&spinedit->oLeft, -4.3f, 0.0f);
	addSubObject(&spinedit->oLeft, &goLeft);
	addSubObject(&spinedit->oSpinEdit, &spinedit->oLeft);

	/* arror right */
	initObjectGroup(&spinedit->oRight);
	setObjectPosition2f(&spinedit->oRight, 3.3f, 0.0f);
	addSubObject(&spinedit->oRight, &goRight);
	addSubObject(&spinedit->oSpinEdit, &spinedit->oRight);

	/* object between arrows */
	addSubObject(&spinedit->oSpinEdit, obj);
#endif

	spinedit->change = change;

	change(spinedit);

#if 0
	/* register callbacks for arrows */
	initPick(&spinedit->pLeft, pickSpinEditLeft, spinedit);
	setObjectPick(&spinedit->oLeft, &spinedit->pLeft);

	initPick(&spinedit->pRight, pickSpinEditRight, spinedit);
	setObjectPick(&spinedit->oRight, &spinedit->pRight);
#endif
}
/*** MenuItem ***/

#define EMPHASIZE_SPEED 10.0f

void updateMenuItem(MenuItem* item, float interval)
{
	if (item->hover)
	{
		item->emphasize += EMPHASIZE_SPEED * interval * (1.0f - item->emphasize);
	}
	else
	{
		item->emphasize += EMPHASIZE_SPEED * interval * (0.0f - item->emphasize);
	}
}

void drawMenuItem(const MenuItem* item)
{
	glPushMatrix();

		glTranslatef(item->position.x, item->position.y, 0.0f);

		switch(item->type)
		{
			case MI_BUTTON:
				drawButton((const Button*) item);
				break;
			default:
				break;
		}

	glPopMatrix();
}

void clickMenuItem(MenuItem* item, float x, float y, MouseEvent event)
{
	item->hover = 0;

	if (x >= item->position.x               && y >= item->position.y &&
	    x <= item->position.x + item->width && y <= item->position.y + item->height)
	{
		switch (event)
		{
			case MOUSE_CLICK:
				switch(item->type)
				{
					case MI_BUTTON:
						clickButton((Button*) item);
						break;
					default:
						break;
				}
				break;
			case MOUSE_MOTION:
				item->hover = 1;
				break;
		}
	}
}

/*** Menu ***/

void initMenu(Menu* menu, int cntItems, MenuItem** items)
{
	menu->cntItems = cntItems;
	menu->items = items;
}

void showMenu(Menu* menu)
{
	int i;

	for (i = 0; i < menu->cntItems; i++)
	{
		menu->items[i]->emphasize = 0.0f;
	}
}

void updateMenu(Menu* menu, float interval)
{
	int i;

	for (i = 0; i < menu->cntItems; i++)
	{
		updateMenuItem(menu->items[i], interval);
	}
}

void drawMenu(const Menu* menu)
{
	int i;

	for (i = 0; i < menu->cntItems; i++)
	{
		drawMenuItem(menu->items[i]);
	}
}

void clickMenu(Menu* menu, float x, float y, MouseEvent event)
{
	int i;

	for (i = 0; i < menu->cntItems; i++)
	{
		clickMenuItem(menu->items[i], x, y, event);
	}
}
