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

#include "ScreenEditorMain.hpp"

#include "process/MainProcess.hpp"
#include "ScreenEditorHelp.hpp"

#include "utils/Callback.hpp"

#include "Main.hpp"

#include "hw/keyboard.hpp"

ScreenEditorMain::ScreenEditorMain(Editor* editor) :
	mParent(editor)
{
	bEdit = Button(6.0f, METHOD_CALLBACK(ScreenEditorMain, clickButtonEdit), "edit", KEY_ENTER);
	addItem(&bEdit);
	
	bSave = Button(5.0f, METHOD_CALLBACK(ScreenEditorMain, clickButtonSave), "save", 's');
	addItem(&bSave);

	bHelp = Button(4.0f, METHOD_CALLBACK(ScreenEditorMain, clickButtonHelp), "help", 'h');
	addItem(&bHelp);

	bQuit = Button(3.0f, METHOD_CALLBACK(ScreenEditorMain, clickButtonQuit), "back", KEY_ESC);
	addItem(&bQuit);
}

ScreenEditorMain::~ScreenEditorMain()
{
  // empty
}

void ScreenEditorMain::clickButtonEdit()
{
	Main::popState();
}

void ScreenEditorMain::clickButtonSave()
{
	mParent->saveLevel();
}

void ScreenEditorMain::clickButtonHelp()
{
	Main::setState(gScreenEditorHelp, false);
}

void ScreenEditorMain::clickButtonQuit()
{
	Main::setState(mParent->mPrevious, true);
}
