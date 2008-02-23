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

#include "mouse.hpp"

#include "math/Vector2.hpp"

typedef void (*funcChange) (const void *self);
typedef void (*funcDraw) (void);

typedef enum
{
	MI_CANVAS,
	MI_LABEL,
	MI_PROGRESS_BAR,
	MI_BUTTON,
	MI_CHECK,
	MI_SPIN_EDIT
} MenuItemType;

class MenuItem
{
public:
  MenuItem();
  virtual ~MenuItem();
  
  virtual void event(float x, float y, MouseEvent event);
  virtual void update(float interval);
  virtual void draw() const;

	MenuItemType type;

	Vector2 position;
	float width;
	float height;

	int hover;
	float emphasize;

protected:
	static float scaleText;
	
private:

};

#endif
