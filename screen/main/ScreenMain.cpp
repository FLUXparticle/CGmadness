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

#include "ScreenMain.hpp"

#include "screen/choose/ScreenChooseGame.hpp"
#include "screen/choose/ScreenChooseInfo.hpp"

#include "process/Choose.hpp"
#include "process/Editor.hpp"

#include "utils/Callback.hpp"

#include "hw/keyboard.hpp"
#include "Main.hpp"

#include <cstdlib>

ScreenMain::ScreenMain()
{
	gScreenChooseInfo = new ScreenChooseInfo(MY_CALLBACK(ScreenMain, clickButtonChooseEditor));

	bCGMadness = Button(6.0f, MY_CALLBACK(ScreenMain, clickButtonCGMadness), "CG Madness", KEY_ENTER);
	addItem(&bCGMadness);
	
	bCGMEditor = Button(4.0f, MY_CALLBACK(ScreenMain, clickButtonCGMEditor), "CGM Editor", 'e');
	addItem(&bCGMEditor);
	
	bQuit = Button(2.0f, MY_CALLBACK(ScreenMain, clickButtonQuit), "Quit", 'q');
	addItem(&bQuit);
}

ScreenMain::~ScreenMain()
{
  // empty
}

void ScreenMain::clickButtonCGMadness()
{
	mChoose = new Choose(gScreenChooseGame);
	Main::setState(mChoose, true);
}

void ScreenMain::clickButtonCGMEditor()
{
	mChoose = new Choose(gScreenChooseInfo);
	Main::setState(mChoose, true);
}

void ScreenMain::clickButtonQuit()
{
	exit(0);
}

void ScreenMain::clickButtonChooseEditor()
{
	Main::setState(mEditor, true);
}
