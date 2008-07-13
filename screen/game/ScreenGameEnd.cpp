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

#include "ScreenGameEnd.hpp"

#include "process/MainProcess.hpp"

#include "Main.hpp"

#include "utils/Callback.hpp"

#include "hw/keyboard.hpp"
#include "Main.hpp"

ScreenGameEnd::ScreenGameEnd(Game* parent) :
	mParent(parent)
{
	hsHighScore = HighScore(3.0f);
	mItems.push_back(&hsHighScore);

	bAgain = Button(2.0f, CALLBACK(ScreenGameEnd, clickButtonAgain), "play again", KEY_ENTER);
	mItems.push_back(&bAgain);
	
	bQuit2 = Button(1.0f, CALLBACK(ScreenGameEnd, clickButtonQuit2), "change level", KEY_ESC);
	mItems.push_back(&bQuit2);
}

ScreenGameEnd::~ScreenGameEnd()
{
  // empty
}

void ScreenGameEnd::clickButtonAgain()
{
	hsHighScore.acceptHighScoreName();
	Main::popState();
	mParent->resetGame();
}

void ScreenGameEnd::clickButtonQuit2()
{
	hsHighScore.acceptHighScoreName();
	mParent->leaveGame();
}
