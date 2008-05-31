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

#ifndef Dispenser_hpp
#define Dispenser_hpp

#include "Process.hpp"

#include <list>

class Dispenser : public Process
{
public:
  Dispenser();
  virtual ~Dispenser();

  void event(const Vector3& position, const Vector3& direction, MouseEvent event);
  
  void update(float interval);

  void preDisplay();
  void draw() const;
  void drawHUD(float width, float height);
  
protected:
	void setProcess(Process* process, bool flush);
	void pushProcess(Process* process);
	void popProcess();
	void popScreen();

private:
	std::list<Process*> mProcessStack;
	Process* mNewProcess;
	bool mPush;
	bool mFlush;
	
};

#endif
