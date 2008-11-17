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

#include "ScreenChooseGame.hpp"

#include "Main.hpp"
#include "process/RaceTheClock.hpp"

#include "utils/Callback.hpp"

#include "hw/keyboard.hpp"

ScreenChooseGame::ScreenChooseGame()
{
	bChooseGame = Button(2.0f, METHOD_CALLBACK(ScreenChooseGame, clickButtonChooseGame), "choose", KEY_ENTER);
	addItem(&bChooseGame);
	
	hsHighScore = HighScore(3.0f);
	addItem(&hsHighScore);
}

ScreenChooseGame::~ScreenChooseGame()
{
  // empty
}

void ScreenChooseGame::clickButtonChooseGame()
{
	Main::setState(mGame, true);
}

