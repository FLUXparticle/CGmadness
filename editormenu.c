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
#include "objects.h"
#include "texture.h"
#include "keyboard.h"
#include "gui.h"

#include <GL/glut.h>

#include <string.h>

#define SCALE_FONT 0.5f

#if 0
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
#endif

void updateEditorMenu(float interval) {
#if 0
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
#endif
}

void showEditorMenu(int menu) {
	glutSetCursor(GLUT_CURSOR_LEFT_ARROW);

#if 0
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
#endif
}

void setEditorMenuPosition(Vector3 pos) {
#if 0
	gEditorMenuPosition = pos;
#endif
}

void drawEditorMenu(void) {
#if 0
	glEnable(GL_LIGHTING);

		setSomeLight();

		glPushMatrix();
			glTranslatef(gEditorMenuPosition.x, gEditorMenuPosition.y, gEditorMenuPosition.z);

			drawObject(&goLogo);
			drawObject(gCurMenu);

		glPopMatrix();

	glDisable(GL_LIGHTING);
#endif
}

void initEditorMenu() {
#if 0
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

	initButton(&bEdit, 6.0f, clickButtonEdit, "Edit");
	initButton(&bSave, 5.0f, clickButtonSave, "Save");
	initButton(&bHelp, 4.0f, clickButtonHelp, "Help");
	initButton(&bQuit, 3.0f, clickButtonQuit, "Quit");

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

	initButton(&bBack, 6.0f - LENGTH(gTextHelp), clickButtonBack, "back");

 	/* response */

	initObjectGroup(&goSuccessfulText);
	initButton(&bSuccessful, 5.0, clickButtonBack, "level saved successfully");

	initObjectGroup(&goFailedText);
	initButton(&bFailed, 5.0, clickButtonBack, "operation failed");
#endif
}
