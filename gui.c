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

static float scaleText = 0.1f * SCALE_FONT;

static unsigned int gTexLeft;
static unsigned int gTexRight;

static int gInitialize = 0;

void drawArrowLeft(void)
{
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, gTexLeft);

		drawSquare();

	glDisable(GL_TEXTURE_2D);
}

void drawArrowRight(void)
{
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, gTexRight);

		drawSquare();

	glDisable(GL_TEXTURE_2D);
}

void initGUI() {
	if (!gInitialize) {
		/* loading arrow textures */
		gTexLeft = loadTexture("data/left.tga", 0);
		gTexRight = loadTexture("data/right.tga", 0);

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

/*** Label ***/

void initLabel(Label* label, float x, float z, int alignRight, char* text)
{
	label->text = text;

	label->item.type = MI_LABEL;

	label->item.width = widthFont3DText(label->text) * scaleText;
	label->item.height = 0.9f;

	if (alignRight)
	{
		label->item.position = vector2(x - label->item.width, z);
	}
	else
	{
		label->item.position = vector2(x, z);
	}
}

void drawLabel(const Label* label)
{
	float scale = scaleText;

	glPushMatrix();

		glScalef(scale, scale, scale);

		drawFont3DText(label->text);

	glPopMatrix();
}

/*** ProgressBar ***/

void initProgressBar(ProgressBar* progressBar, float z, float* progress)
{
	progressBar->progress = progress;

	progressBar->item.type = MI_PROGRESS_BAR;

	progressBar->item.width = 8.0f;
	progressBar->item.height = 0.9f;

	progressBar->item.position = vector2(0.0f, z);
}

void drawProgressBar(const ProgressBar* progressBar)
{
	glPushMatrix();

		glScalef(progressBar->item.width / 2.0f, progressBar->item.height / 2.0f, 1.0f);

		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

			drawSquare();

		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);


		glScalef(*progressBar->progress, 1.0f, 1.0f);

		drawSquare();

	glPopMatrix();
}

/*** Button ***/

void initButton(Button* button, float z, funcClick click, char* text)
{
	button->click = click;
	button->text = text;

	button->item.type = MI_BUTTON;

	button->item.width = widthFont3DText(button->text) * scaleText;
	button->item.height = 0.9f;

	button->item.position = vector2(-button->item.width / 2.0f, z);
}

void eventButton(Button* button)
{
	button->click();
}

void drawButton(const Button* button)
{
	float scale = scaleText * (1.0f + 0.1f * button->item.emphasize);

	glPushMatrix();

		glTranslatef(-0.5f * button->item.width * 0.1f * button->item.emphasize, 0.0f, 0.0f);
		glScalef(scale, scale, scale);

		drawFont3DText(button->text);

	glPopMatrix();
}

/*** Check ***/

void setCheck(Check* check, int value)
{
	check->value = value;
	check->change(check);
}

void initCheck(Check* check, float z, funcChange change, char* text)
{
	check->text = text;
	check->change = change;

	check->item.type = MI_CHECK;

	check->item.width = widthFont3DText(check->text) * scaleText;
	check->item.height = 0.9f;

	check->item.position = vector2(-check->item.width / 2.0f, z);

	setCheck(check, 1);
}

void drawCheck(const Check* check)
{
	float scale = scaleText * (1.0f + 0.1f * check->item.emphasize);

	glPushMatrix();

		glTranslatef(-0.5f * check->item.width * 0.1f * check->item.emphasize, 0.0f, 0.0f);
		glScalef(scale, scale, scale);

		if (check->value) {
			glColor3f(1.0f, 1.0f, 1.0f);
		} else {
			glColor3f(0.5f, 0.5f, 0.5f);
		}

			drawFont3DText(check->text);

		glColor3f(1.0f, 1.0f, 1.0f);

	glPopMatrix();
}

void eventCheck(Check* check)
{
	setCheck(check, !check->value);
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

#if 0
void init3dSpinEdit(SpinEdit* spinedit, int value, int min, int max, float z, Object* obj, funcChange change) {
	spinedit->value = value;
	spinedit->minValue = min;
	spinedit->maxValue = max;

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

	spinedit->change = change;

	change(spinedit);

	/* register callbacks for arrows */
	initPick(&spinedit->pLeft, pickSpinEditLeft, spinedit);
	setObjectPick(&spinedit->oLeft, &spinedit->pLeft);

	initPick(&spinedit->pRight, pickSpinEditRight, spinedit);
	setObjectPick(&spinedit->oRight, &spinedit->pRight);
}
#endif

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
			case MI_LABEL:
				drawLabel((const Label*) item);
				break;
			case MI_PROGRESS_BAR:
				drawProgressBar((const ProgressBar*) item);
				break;
			case MI_BUTTON:
				drawButton((const Button*) item);
				break;
			case MI_CHECK:
				drawCheck((const Check*) item);
				break;
		}

	glPopMatrix();
}

void eventMenuItem(MenuItem* item, float x, float y, MouseEvent event)
{
	item->hover = 0;

	if (item->type == MI_BUTTON || MI_CHECK)
	{
		if (x >= item->position.x               && y >= item->position.y &&
				x <= item->position.x + item->width && y <= item->position.y + item->height)
		{
			switch (event)
			{
				case MOUSE_CLICK:
					switch(item->type)
					{
						case MI_BUTTON:
							eventButton((Button*) item);
							break;
						case MI_CHECK:
							eventCheck((Check*) item);
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

void eventMenu(Menu* menu, float x, float y, MouseEvent event)
{
	int i;

	for (i = 0; i < menu->cntItems; i++)
	{
		eventMenuItem(menu->items[i], x, y, event);
	}
}
