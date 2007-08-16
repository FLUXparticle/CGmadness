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
	{ "Cursor", "Move" },
	{ "Shift", "Multi Select" },
	{ "W A S D", "Pitch" },
	{ "R F", "Hight" },
	{ "Y X C V B N", "Camera" },
	{ "1 2", "Start Finish" },
	{ "Esc", "Menu" },
};

static Vector3 gEditorMenuPosition;

static Object goLogo;
static Object goMainMenu;
static Object goHelpMenu;

static Object goSuccessfulText;
static Object goFailedText;

static Object* gCurMenu;

/* events */

static void clickButtonEdit(void) {
	resumeEditor();
}

static void clickButtonSave(void) {
	saveLevel();
}

static void clickButtonHelp(void) {
	gCurMenu = &goHelpMenu;
}

static void clickButtonQuit(void) {
	exit(0);
}

static void clickButtonBack(void) {
	gCurMenu = &goMainMenu;
}

void updateEditorMenu(float interval) {
	if (gCurMenu == &goMainMenu) {
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
	} else {
		if (wasKeyPressed(KEY_ENTER) || wasKeyPressed(KEY_ESC)) {
			clickButtonBack();
		}
	}
}

void showEditorMenu(int menu) {
	glutSetCursor(GLUT_CURSOR_LEFT_ARROW);

	switch (menu) {
	case 0:
		gCurMenu = &goMainMenu;
		break;
	case 1:
		gCurMenu = &goSuccessfulText;
		break;
	case 2:
		gCurMenu = &goFailedText;
		break;
	}
}

void setEditorMenuPosition(Vector3 pos) {
	gEditorMenuPosition = pos;
}

void drawEditorMenu(void) {
	glEnable(GL_LIGHTING);

		setSomeLight();

		glPushMatrix();
			glTranslatef(gEditorMenuPosition.x, gEditorMenuPosition.y, gEditorMenuPosition.z);

			drawObject(&goLogo);
			drawObject(gCurMenu);

		glPopMatrix();

	glDisable(GL_LIGHTING);
}

void pickEditorMenu(void) {
	glPushMatrix();
		glTranslatef(gEditorMenuPosition.x, gEditorMenuPosition.y, gEditorMenuPosition.z);

		pickObject(gCurMenu);

	glPopMatrix();
}

void initEditorMenu() {
	static Button bEdit;
	static Button bSave;
	static Button bHelp;
	static Button bQuit;
	static Button bBack;
	static Button bSuccessful;
	static Button bFailed;

	static Object oTextHelp[2 * LENGTH(gTextHelp)];

	int i;

	initGUI();

	/*
	 * put all together
	 */

	/* menu logo */
	initObject(&goLogo, drawSquare);
	goLogo.texture = loadTexture("data/logo.tga", 0);
	setObjectPosition3f(&goLogo, 0.0f, 0.0f, 8.0f);
	setObjectScale3f(&goLogo, 4.0f, 1.0f, 1.0f);

	rotateObjectX(&goLogo, 90.0f);

	/* main menu */
	initObjectGroup(&goMainMenu);

	init3dButton(&bEdit, 6.0f, clickButtonEdit, "Edit");
  addSubObject(&goMainMenu, &bEdit.oButton);

	init3dButton(&bSave, 5.0f, clickButtonSave, "Save");
  addSubObject(&goMainMenu, &bSave.oButton);

	init3dButton(&bHelp, 4.0f, clickButtonHelp, "Help");
  addSubObject(&goMainMenu, &bHelp.oButton);

	init3dButton(&bQuit, 3.0f, clickButtonQuit, "Quit");
  addSubObject(&goMainMenu, &bQuit.oButton);

	/* help text */
	initObjectGroup(&goHelpMenu);

	for (i = 0; i < LENGTH(gTextHelp); i++) {
		float z = 6.0f - i;
		float length;

		{
			Object* o = &oTextHelp[2 * i];

			length = makeTextObject(o, gTextHelp[i].left) * SCALE_FONT;
			setObjectPosition3f(o, -5.0f, 0.0f, z);
			setObjectScalef(o, SCALE_FONT);
			rotateObjectX(o, 90.0f);

	  	addSubObject(&goHelpMenu, o);
		}

		{
			Object* o = &oTextHelp[2 * i + 1];

			length = makeTextObject(o, gTextHelp[i].right) * SCALE_FONT;
			setObjectPosition3f(o, 5.0f - length, 0.0f, z);
			setObjectScalef(o, SCALE_FONT);
			rotateObjectX(o, 90.0f);

	  	addSubObject(&goHelpMenu, o);
		}
	}

	init3dButton(&bBack, 6.0f - LENGTH(gTextHelp), clickButtonBack, "back");
 	addSubObject(&goHelpMenu, &bBack.oButton);

 	/* response */

	initObjectGroup(&goSuccessfulText);
	init3dButton(&bSuccessful, 5.0, clickButtonBack, "level saved successfully");
 	addSubObject(&goSuccessfulText, &bSuccessful.oButton);

	initObjectGroup(&goFailedText);
	init3dButton(&bFailed, 5.0, clickButtonBack, "operation failed");
 	addSubObject(&goFailedText, &bFailed.oButton);
}
