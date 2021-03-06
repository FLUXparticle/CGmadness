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

#include "Atlas.hpp"

#include "macros.hpp"

#include <cstdio>
#include <cassert>

Atlas* sgAtlas;

static int nextPowerOfTwo(int i)
{
	int j = 1;

	while (j < i)
	{
		j <<= 1;
	}

	return j;
}

int Atlas::getCntAllocatedSubLightMaps()
{
	return gMaxSubLightMaps;
}

Atlas::Atlas(int cntSubTextures)
{
	gRows = 1;
	do
	{
		gRows <<= 1;
		gCols = ((cntSubTextures - 1) / gRows) + 1;
	} while (gCols > gRows);

	gCols = nextPowerOfTwo(gCols);

	gLightMapSizeX = gCols * LIGHT_MAP_SIZE;
	gLightMapSizeY = gRows * LIGHT_MAP_SIZE;

	gLightMapData = new float[gLightMapSizeX * gLightMapSizeY];

	gMaxSubLightMaps = cntSubTextures;

	PRINT_INT(gLightMapSizeX);
	PRINT_INT(gLightMapSizeY);
	PRINT_INT(cntSubTextures);
}

Atlas::~Atlas()
{
	delete[] gLightMapData;

	gCols = 0;
	gRows = 0;
}

float& Atlas::getSubLightMapPixel(int index, int sx, int sy)
{
	int x = (index % gCols) * LIGHT_MAP_SIZE + sx;
	int y = (index / gCols) * LIGHT_MAP_SIZE + sy;
	return gLightMapData[y * gLightMapSizeX + x];
}

Vector2 Atlas::transformSubCoords(int index, const Vector2 coords)
{
	return Vector2(((index % gCols) + coords.x) / gCols,
								 ((index / gCols) + coords.y) / gRows);
}

void Atlas::getSubLightMap(int index, float data[SIZEOF_LIGHT_MAP])
{
	int i = 0;

	for (int y = 0; y < LIGHT_MAP_SIZE; y++)
	{
		for (int x = 0; x < LIGHT_MAP_SIZE; x++)
		{
			data[i++] = getSubLightMapPixel(index, x, y);
		}
	}
}

void Atlas::setSubLightMap(int index, const float data[SIZEOF_LIGHT_MAP])
{
	int i = 0;

	for (int y = 0; y < LIGHT_MAP_SIZE; y++)
	{
		for (int x = 0; x < LIGHT_MAP_SIZE; x++)
		{
			getSubLightMapPixel(index, x, y) = data[i++];
		}
	}
}

/*****/

void Atlas::setLightMap(int index, int x, int y, float value)
{
	assert(x >= 0 && x < LIGHT_MAP_SIZE);
	assert(y >= 0 && y < LIGHT_MAP_SIZE);

	getSubLightMapPixel(index, x, y) = value;
}

void Atlas::transformCoords(int index, Vector2& coords)
{
	if (gCols > 0 && gRows > 0)
	{
		assert(coords.x >= 0.0f && coords.x <= 1.0f);
		assert(coords.y >= 0.0f && coords.y <= 1.0f);

		coords = transformSubCoords(index, coords);
	}
}
