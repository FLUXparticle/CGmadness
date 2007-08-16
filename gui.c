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
#include "pick.h"
#include "objects.h"
#include "texture.h"

#include <GL/glut.h>

#include <string.h>

#define SCALE_FONT 0.5f

/*** common ***/

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

void pickButton(void* data) {
	Button* button = data;
	button->click();
}

void init3dButton(Button* button, float z, funcClick click, char* text) {
	initTextObject(&button->oButton, text, z);

	button->click = click;

	if (click) {
		initPick(&button->pButton, pickButton, button);
		setObjectPick(&button->oButton, &button->pButton);
	}
}

/*** Check ***/

void setCheck(Check* check, int value) {
	check->value = value;

	if (check->value) {
		setObjectGroupColor(&check->oCheck, 1.0f, 1.0f, 1.0f);
	} else {
		setObjectGroupColor(&check->oCheck, 0.5f, 0.5f, 0.5f);
	}

	check->change(check);
}

void pickCheck(void* data) {
	Check* check = data;
	setCheck(check, !check->value);
}

void init3dCheck(Check* check, float z, funcChange change, char* text) {
	initTextObject(&check->oCheck, text, z);

	check->change = change;

	setCheck(check, 1);

	initPick(&check->pCheck, pickCheck, check);
	setObjectPick(&check->oCheck, &check->pCheck);
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
