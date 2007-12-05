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
#include "keyboard.h"

#include "functions.h"
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

		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

			glPushMatrix();

				glScalef(0.5f, 0.5f, 1.0f);

				drawSquare();

			glPopMatrix();

		glDisable(GL_BLEND);

	glDisable(GL_TEXTURE_2D);
}

void drawArrowRight(void)
{
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, gTexRight);

		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

			glPushMatrix();

				glScalef(0.5f, 0.5f, 1.0f);

				drawSquare();

			glPopMatrix();

		glDisable(GL_BLEND);

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

/*** Canvas ***/

void initCanvas(Canvas* canvas, float z, float width, float height, funcUpdate customUpdate, funcDraw customDraw)
{
	canvas->customUpdate = customUpdate;
	canvas->customDraw = customDraw;

	canvas->item.type = MI_CANVAS;

	canvas->item.width = width;
	canvas->item.height = height;

	canvas->item.position = vector2(-canvas->item.width / 2.0f, z);
}

void drawCanvas(const Canvas* canvas)
{
	canvas->customDraw();
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

void initButton(Button* button, float z, funcClick click, char* text, int shortcut)
{
	button->click = click;
	button->text = text;
	button->shortcut = shortcut;

	button->item.type = MI_BUTTON;

	button->item.width = widthFont3DText(button->text) * scaleText;
	button->item.height = 0.5f;

	button->item.position = vector2(-button->item.width / 2.0f, z);
}

void eventButton(Button* button, MouseEvent event)
{
	switch (event)
	{
		case MOUSE_CLICK:
			button->click();
			break;
		default:
			button->item.hover = 1;
			break;
	}
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
	check->item.height = 0.5f;

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

void eventCheck(Check* check, MouseEvent event)
{
	switch (event)
	{
		case MOUSE_CLICK:
			setCheck(check, !check->value);
			break;
		default:
			check->item.hover = 1;
			break;
	}
}

/*** SpinEdit ***/

/*
 * TODO split SpinEdit into two seperate MenuItems
 */

void initSpinEdit(SpinEdit* spinEdit, int value, int min, int max, float z, funcDraw draw, funcChange change)
{
	spinEdit->item.type = MI_SPIN_EDIT;

	spinEdit->item.width = 4.3;
	spinEdit->item.height = 1.0f;

	spinEdit->item.position = vector2(-spinEdit->item.width / 2.0f, z - 0.5);

	spinEdit->value = value;
	spinEdit->minValue = min;
	spinEdit->maxValue = max;

	/* object between arrows */
	spinEdit->draw = draw;
	spinEdit->change = change;

	spinEdit->change(spinEdit);
}

void drawSpinEdit(const SpinEdit* spinEdit)
{
	float scale = 1.0f + 0.2f * spinEdit->item.emphasize;

	glPushMatrix();

		glTranslatef(0.5f, 0.5f, 0.0f);

		glPushMatrix();

			if (spinEdit->side == -1)
			{
				glScalef(scale, scale, 1.0f);
			}

			drawArrowLeft();

		glPopMatrix();

		glTranslatef(1.65f, 0.0f, 0.0f);

		glPushMatrix();

			glScalef(0.5f, 0.5f, 0.5f);

			glPushAttrib(GL_CURRENT_BIT | GL_LIGHTING_BIT);

				spinEdit->draw();

			glPopAttrib();

		glPopMatrix();

		glTranslatef(1.65f, 0.0f, 0.0f);

		glPushMatrix();

			if (spinEdit->side == 1)
			{
				glScalef(scale, scale, 1.0f);
			}

			drawArrowRight();

		glPopMatrix();

	glPopMatrix();
}

void changeSpinEdit(SpinEdit* spinEdit, int change)
{
	spinEdit->value = clampi(spinEdit->value + change, spinEdit->minValue, spinEdit->maxValue);
	spinEdit->change(spinEdit);
}

void eventSpinEdit(SpinEdit* spinEdit, float x, float y, MouseEvent event)
{
	int side;
	if (x < -1.15f)
	{
		side = -1;
	}
	else if (x > 1.15f)
	{
		side = 1;
	}
	else
	{
		side = 0;
	}

	if (side != 0)
	{
		spinEdit->side = side;
	}

	switch (event)
	{
		case MOUSE_CLICK:
			changeSpinEdit(spinEdit, side);
			break;
		default:
			spinEdit->item.hover = (side != 0);
			break;
	}

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
	
	/*
	 * TODO  needs rewrite of event-system
	 */
	if (item->type == MI_BUTTON)
	{
		Button* button = (Button*) item;
		if (wasKeyPressed(button->shortcut))
		{
			button->click();
		}
	}
	else if (item->type == MI_SPIN_EDIT)
	{
		SpinEdit* spinEdit = (SpinEdit*) item;
		if (wasCursorPressed(CURSOR_LEFT))
		{
			changeSpinEdit(spinEdit, -1);
		}
		else if (wasCursorPressed(CURSOR_RIGHT))
		{
			changeSpinEdit(spinEdit, +1);
		}
	}
	else if (item->type == MI_CANVAS)
	{
		Canvas* canvas = (Canvas*) item;
		canvas->customUpdate(interval);
	} 
}

void drawMenuItem(const MenuItem* item)
{
	glPushMatrix();

		glTranslatef(item->position.x, item->position.y, 0.0f);

		switch(item->type)
		{
			case MI_CANVAS:
				drawCanvas((const Canvas*) item);
				break;
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
			case MI_SPIN_EDIT:
				drawSpinEdit((const SpinEdit*) item);
				break;
		}

	glPopMatrix();
}

void eventMenuItem(MenuItem* item, float x, float y, MouseEvent event)
{
	item->hover = 0;

	if (x >= item->position.x               && y >= item->position.y &&
			x <= item->position.x + item->width && y <= item->position.y + item->height)
	{
		switch(item->type)
		{
			case MI_BUTTON:
				eventButton((Button*) item, event);
				break;
			case MI_CHECK:
				eventCheck((Check*) item, event);
				break;
			case MI_SPIN_EDIT:
				eventSpinEdit((SpinEdit*) item, x, y, event);
				break;
			default:
				break;
		}
	}
}

/*** Screen ***/

void initScreen(Screen* screen, int cntItems, MenuItem** items)
{
	screen->cntItems = cntItems;
	screen->items = items;
}

void prepareScreen(Screen* screen)
{
	int i;

	for (i = 0; i < screen->cntItems; i++)
	{
		screen->items[i]->emphasize = 0.0f;
	}
}

void updateScreen(Screen* screen, float interval)
{
	int i;

	for (i = 0; i < screen->cntItems; i++)
	{
		updateMenuItem(screen->items[i], interval);
	}
}

void drawScreen(const Screen* screen)
{
	int i;

	for (i = 0; i < screen->cntItems; i++)
	{
		drawMenuItem(screen->items[i]);
	}
}

void eventScreen(Screen* screen, float x, float y, MouseEvent event)
{
	int i;

	for (i = 0; i < screen->cntItems; i++)
	{
		eventMenuItem(screen->items[i], x, y, event);
	}
}
