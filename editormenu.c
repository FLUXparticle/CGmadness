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

#include "editormenu.h"

#include "editor.h"

#include "text.h"
#include "graph.h"
#include "pick.h"
#include "objects.h"
#include "texture.h"
#include "keyboard.h"
#include "gui.h"

#include <GL/glut.h>

#include <string.h>

#define SCALE_FONT 0.5f

typedef struct {
	char* left;
	char* right;
} LeftRight;

/*
 * help text
 */
static LeftRight gTextHelp[] = {
	{ "W A S D", "Pitch" },
	{ "R F", "Hight" },
	{ "Y X C V B N", "Camera" },
	{ "1 2", "Start Finish" },
	{ "Esc", "Menu" },
};

static Object goHelpText;
static Object goMainText;

/* events */

static void clickButtonEdit(void) {
	resumeEditor();
}

static void clickButtonSave(void) {
	saveLevel();
}

static void clickButtonHelp(void) {
	goHelpText.visible = 1;
	goMainText.visible = 0;
}

static void clickButtonQuit(void) {
	exit(0);
}

static void clickButtonBack(void) {
	goHelpText.visible = 0;
	goMainText.visible = 1;
}

void updateEditorMenu(float interval) {
	if (goMainText.visible) {
		if (wasKeyPressed(KEY_ESC) || wasKeyPressed(KEY_ENTER)) {
			clickButtonEdit();
		}
		
		if (wasKeyPressed('s')) {
			clickButtonSave();
		}

		if (wasKeyPressed('h')) {
			clickButtonHelp();
		}

		if (wasKeyPressed('q')) {
			clickButtonQuit();
		}
	} else if (goHelpText.visible) {
		if (wasKeyPressed(KEY_ESC)) {
			clickButtonBack();
		}
	}
}

void showEditorMenu(void) {
	glutSetCursor(GLUT_CURSOR_LEFT_ARROW);
}

void initEditorMenu(Object* obj) {
	static Button bEdit;
	static Button bSave;
	static Button bHelp;
	static Button bQuit;
	static Button bBack;
	
	static Object oLogo;	
	static Object oTextHelp[2 * LENGTH(gTextHelp)];

	int i;
	
	initGUI();

	/*
	 * put all together
	 */

	initObjectGroup(obj);
	
	/* menu logo */
	initObject(&oLogo, drawSquare);
	oLogo.texture = loadTexture("data/logo.tga", 0);
	setObjectPosition3f(&oLogo, 0.0f, 0.0f, 8.0f);
	setObjectScale3f(&oLogo, 4.0f, 1.0f, 1.0f);

	rotateObjectX(&oLogo, 90.0f);
	addSubObject(obj, &oLogo);

	/* main menu */
	initObjectGroup(&goMainText);

	init3dButton(&bEdit, 6.0f, clickButtonEdit, "Edit");
  addSubObject(&goMainText, &bEdit.oButton);
	
	init3dButton(&bSave, 5.0f, clickButtonSave, "Save");
  addSubObject(&goMainText, &bSave.oButton);
	
	init3dButton(&bHelp, 4.0f, clickButtonHelp, "Help");
  addSubObject(&goMainText, &bHelp.oButton);
	
	init3dButton(&bQuit, 3.0f, clickButtonQuit, "Quit");
  addSubObject(&goMainText, &bQuit.oButton);

	addSubObject(obj, &goMainText);

	/* help text */
	initObjectGroup(&goHelpText);
	
	goHelpText.visible = 0;

	for (i = 0; i < LENGTH(gTextHelp); i++) {
		float z = 6.0f - i;
		float length;

		{
			Object* o = &oTextHelp[2 * i];

			length = makeTextObject(o, gTextHelp[i].left) * SCALE_FONT;
			setObjectPosition3f(o, -5.0f, 0.0f, z);
			setObjectScalef(o, SCALE_FONT);
			rotateObjectX(o, 90.0f);
			
	  	addSubObject(&goHelpText, o);
		}

		{
			Object* o = &oTextHelp[2 * i + 1];
			
			length = makeTextObject(o, gTextHelp[i].right) * SCALE_FONT;
			setObjectPosition3f(o, 5.0f - length, 0.0f, z);
			setObjectScalef(o, SCALE_FONT);
			rotateObjectX(o, 90.0f);
			
	  	addSubObject(&goHelpText, o);
		}
	}

	init3dButton(&bBack, 6.0f - LENGTH(gTextHelp), clickButtonBack, "back");
 	addSubObject(&goHelpText, &bBack.oButton);

	addSubObject(obj, &goHelpText);
}
