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

#ifndef _atlas_hpp_
#define _atlas_hpp_

#include "math/Vector2.hpp"
#include "math/Vector3.hpp"

#include "Color.hpp"

#include GL_H

#define LIGHT_MAP_SIZE 8

#define SIZEOF_LIGHT_MAP (LIGHT_MAP_SIZE * LIGHT_MAP_SIZE)

void initAtlas(int cntSubTextures);

int getCntAllocatedSubLightMaps();

void destroyAtlas();

void lightMapToTexture(unsigned int texID);

void getSubLightMap(int index, float data[SIZEOF_LIGHT_MAP]);
void setSubLightMap(int index, const float data[SIZEOF_LIGHT_MAP]);

/*****/

int allocSubAtlas();

void setLightMap(int index, int x, int y, float value);

void transformCoords(int index, Vector2& coords);

#endif
