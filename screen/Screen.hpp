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

#ifndef Screen_hpp
#define Screen_hpp

#include "process/Process.hpp"

#include "gui/MenuItem.hpp"

#include "Main.hpp"

#include <list>

class Screen : public Process
{
public:
  Screen();
  virtual ~Screen();

  void start(Process* previous, bool push);

  virtual void show();

  void popScreen();

  void update(float interval);
  virtual void customUpdate(float interval);

  void draw() const;

protected:
	Process* mPrevious;

	void addItem(MenuItem* item);

private:
	static unsigned int gTexLogo;

	static void drawLogo();

private:
	typedef std::list<MenuItem*> MenuItems;

	MenuItems mItems;
	MenuItems mInteractiveItems;
	MenuItems::iterator mSelectedItem;

	float mAnimationTime;

};

#endif
