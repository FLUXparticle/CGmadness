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

#ifndef ScreenMain_hpp
#define ScreenMain_hpp

#include "screen/Screen.hpp"

#include "gui/Button.hpp"

#include "utils/Singleton.hpp"
#include "utils/SmartPointer.hpp"

class ScreenMain : public Screen
{
public:
  ScreenMain();
  virtual ~ScreenMain();

private:
	Button bRaceTheClock;
	Button bSandbox;
	Button bCGMEditor;
	Button bHelp;
	Button bQuit;

	Singleton<class ScreenChooseGame> gScreenChooseGame;
	SmartPointer<class ScreenChooseInfo> gScreenChooseInfo;
	Singleton<class ScreenGameHelp> gScreenGameHelp;
	SmartPointer<class Choose> mChoose;
	Singleton<class Editor> mEditor;

	void clickButtonRaceTheClock();
	void clickButtonCGMEditor();
	void clickButtonHelp();
	void clickButtonQuit();

	void clickButtonChooseEditor();

};

#endif
