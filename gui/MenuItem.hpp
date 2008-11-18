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

#ifndef MenuItem_hpp
#define MenuItem_hpp

#include "hw/mouse.hpp"

#include "math/Vector2.hpp"

typedef void (*funcChange) (const void *self);
typedef void (*funcDraw) (void);

class MenuItem
{
public:
  MenuItem(bool interactive = true);
  virtual ~MenuItem();

  virtual void update(float interval);
  virtual bool updateSelected(float interval);
  virtual void draw() const;

	Vector2 position;
	float width;
	float height;

	bool hover;
	float emphasize;

	bool interactive() const;

protected:
	static float scaleText;

private:
	bool mInteractive;

};

inline bool MenuItem::interactive() const
{
	return mInteractive;
}

#endif
