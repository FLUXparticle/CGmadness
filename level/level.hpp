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

#ifndef level_hpp
#define level_hpp

#include "types.hpp"

extern const int sgEdgeX[4];
extern const int sgEdgeY[4];

void initLevel();
void destroyLevel();

Block& getBlock(int x, int y);
Block& getBlock(int index);
void checkBlock(Block& b);
const Square& getRoofSquare(int x, int y);
const SideFace& getSideFace(int x, int y, int side);

#endif
