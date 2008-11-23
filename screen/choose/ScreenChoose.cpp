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

#include "ScreenChoose.hpp"

#include "utils/Callback.hpp"
#include "utils/Singleton.hpp"

#include "process/MainProcess.hpp"
#include "Main.hpp"

#include "level/level.hpp"

#include "hw/keyboard.hpp"

static void changeLevelChooser(const void *self)
{
	Singleton<LevelLoader> gLevelLoader;
	
	const SpinEdit *spinedit = (const SpinEdit *) self;

	gLevelLoader->loadLevelByID(spinedit->value);
	
	sgLevel.lastPlayerIndex = MAX_SCORE_COLS;
}

static void drawMenuLevel()
{
	/* empty */
}

ScreenChoose::ScreenChoose()
{
	gseLevel = SpinEdit(0, 0, mLevelLoader->maxID(), 7.0, 5.0f, drawMenuLevel, changeLevelChooser);
	addItem(&gseLevel);

	bBack = Button(1.0f, METHOD_CALLBACK(ScreenChoose, clickButtonBack), "back", KEY_ESC);
	addItem(&bBack);
}

ScreenChoose::~ScreenChoose()
{
  // empty
}

void ScreenChoose::show()
{
	Screen::show();
	changeLevelChooser(&gseLevel);
}

void ScreenChoose::clickButtonBack()
{
	mLevelLoader->loadLevelByID(-1);
	
	Singleton<MainProcess> main;
	Main::setState(main, true);
}
