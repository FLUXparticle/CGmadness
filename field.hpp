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

#ifndef _field_hpp_
#define _field_hpp_

#include "color.hpp"

#include "vector.hpp"

extern Vector3 *sgVertices;
extern Vector3 *sgNormals;

void setSquareColor(int q, Color4 col);

void initGameField(void);
void destroyGameField(void);

void updateGameField(void);

void drawGameField(int ballReflection);

void getVertIndex(int x, int y, int *start, int *end);

#endif
