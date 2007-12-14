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

#include "editormenu.h"

#include "editor.h"
#include "main.h"
#include "menumanager.h"

#include "text.h"
#include "objects.h"
#include "texture.h"
#include "keyboard.h"
#include "gui.h"

#include <GL/glut.h>

#include <stdlib.h>
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

static Screen gScreenEditor;
static Screen gScreenHelp;
static Screen gScreenSuccessful;
static Screen gScreenFailed;

/* events */

static void clickButtonEdit(void) {
	popScreen();
	resumeEditor();
}

static void clickButtonSave(void) {
	saveLevel();
}

static void clickButtonHelp(void) {
	pushScreen(&gScreenHelp);
}

static void clickButtonBack(void) {
	popScreen();
}

static void clickButtonQuit(void) {
	popScreen();
	stopEditor();
	setMainState(STATE_MAIN);
}

void showEditorScreen(int menu) {
	switch (menu) {
	case 0:
		pushScreen(&gScreenEditor);
		break;
	case 1:
		pushScreen(&gScreenSuccessful);
		break;
	case 2:
		pushScreen(&gScreenFailed);
		break;
	}
}

void initEditorMenu() {
	static Button bEdit;
	static Button bSave;
	static Button bHelp;
	static Button bQuit;
	static Button bBack;
	static Button bSuccessful;
	static Button bFailed;

	static Label lTextHelp[2 * LENGTH(gTextHelp)];

	static MenuItem* itemsEditor[] =
	{
			&bEdit.item,
			&bSave.item,
			&bHelp.item,
			&bQuit.item
	};

	static MenuItem* itemsHelp[LENGTH(lTextHelp) + 1];
	
	static MenuItem* itemsSuccessful[] =
	{
		&bSuccessful.item,
	};

	static MenuItem* itemsFailed[] =
	{
		&bFailed.item,
	};

	int i;

	initGUI();

	/*
	 * put all together
	 */

	/* main menu */
	initButton(&bEdit, 6.0f, clickButtonEdit, "edit", KEY_ENTER);
	initButton(&bSave, 5.0f, clickButtonSave, "save", 's');
	initButton(&bHelp, 4.0f, clickButtonHelp, "help", 'h');
	initButton(&bQuit, 3.0f, clickButtonQuit, "back", KEY_ESC);

	INIT_SCREEN(&gScreenEditor, itemsEditor);
	
	/* help menu */
	for (i = 0; i < LENGTH(lTextHelp); i++)
	{
		int row = i / 2;
		int col = i % 2;
		float z = 6.0f - row;

		initLabel(&lTextHelp[i], col ? 5.0f : -5.0f, z, col, col ? gTextHelp[row].right : gTextHelp[row].left);

		itemsHelp[i] = &lTextHelp[i].item;
	}

	initButton(&bBack, 6.0f - LENGTH(gTextHelp), clickButtonBack, "back", KEY_ESC);

	itemsHelp[LENGTH(lTextHelp)] = &bBack.item;

	INIT_SCREEN(&gScreenHelp, itemsHelp);

 	/* response */

	initButton(&bSuccessful, 5.0, clickButtonBack, "level saved successfully", KEY_ENTER);
	INIT_SCREEN(&gScreenSuccessful, itemsSuccessful);

	initButton(&bFailed, 5.0, clickButtonBack, "operation failed", KEY_ENTER);
	INIT_SCREEN(&gScreenFailed, itemsFailed);
}
