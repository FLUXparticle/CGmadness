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
#include "process/Editor.hpp"

#include "MenuManager.hpp"

#include "utils/Callback.hpp"

#include "hw/keyboard.hpp"
#include "Main.hpp"

ScreenMain::ScreenMain()
{
	gScreenChooseInfo = new ScreenChooseInfo(CALLBACK(ScreenMain, clickButtonChooseEditor));

	bCGMadness = Button(6.0f, CALLBACK(ScreenMain, clickButtonCGMadness), "CG Madness", KEY_ENTER);
	mItems.push_back(&bCGMadness);
	
	bCGMEditor = Button(4.0f, CALLBACK(ScreenMain, clickButtonCGMEditor), "CGM Editor", 'e');
	mItems.push_back(&bCGMEditor);
	
	bQuit = Button(2.0f, CALLBACK(ScreenMain, clickButtonQuit), "Quit", 'q');
	mItems.push_back(&bQuit);
}

ScreenMain::~ScreenMain()
{
  // empty
}

void ScreenMain::clickButtonCGMadness()
{
	gMenuManager->pushScreen(gScreenChooseGame);
}

void ScreenMain::clickButtonCGMEditor()
{
	gMenuManager->pushScreen(gScreenChooseInfo);
}

void ScreenMain::clickButtonQuit()
{
	exit(0);
}

void ScreenMain::clickButtonChooseEditor()
{
	Main::setState(mEditor);
}
