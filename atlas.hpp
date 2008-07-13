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
#include "Color.hpp"

#define LIGHT_MAP_SIZE 8

#define SIZEOF_LIGHT_MAP (LIGHT_MAP_SIZE * LIGHT_MAP_SIZE)

typedef struct
{
	int sizeX;
	int sizeY;
	int idxSubLightMap;
} SubAtlas;

void initAtlas(int cntSubTextures);

int getCntAllocatedSubLightMaps(void);

void destroyAtlas(void);

void getAtlasInfo(unsigned int* sizeX, unsigned int* sizeY, const float** data);

void getSubLightMap(int index, float data[SIZEOF_LIGHT_MAP]);
void setSubLightMap(int index, const float data[SIZEOF_LIGHT_MAP]);

/*****/

const float* getLightMapData();

void allocSubAtlas(SubAtlas * subAtlas, int sizeX, int sizeY);

void setLightMap(SubAtlas * subAtlas, int x, int y, float value);

Vector2 transformCoords(const SubAtlas * subAtlas, const Vector2 coords);

#endif
