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

#ifndef MenuManager_hpp
#define MenuManager_hpp

#include "process/Process.hpp"

#include <stack>

class Screen;

class MenuManager : public Process
{
public:
  MenuManager();
  virtual ~MenuManager();

  void pushScreen(Screen* screen);
  void popScreen();
  
  void update(float interval);
  void event(const Vector3& position, const Vector3& direction, MouseEvent event);
  
  void draw() const;
  
private:
	std::stack<Screen*> mScreenStack;

};

#endif
