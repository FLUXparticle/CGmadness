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

#ifndef ScreenGameEnd_hpp
#define ScreenGameEnd_hpp

#include "screen/Screen.hpp"

#include "screen/choose/HighScore.hpp"

#include "process/Game.hpp"

#include "gui/Button.hpp"

class ScreenGameEnd : public Screen
{
public:
  ScreenGameEnd(Game* parent);
  virtual ~ScreenGameEnd();

  const String& lastEntry() const;

private:
	HighScore hsHighScore;
	Button bAgain;
	Button bQuit2;

	Game* mParent;

	void clickButtonAgain();
	void clickButtonQuit2();

};

inline const String& ScreenGameEnd::lastEntry() const
{
	return hsHighScore.lastEntry();
}

#endif
