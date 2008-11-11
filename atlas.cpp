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

#include "atlas.hpp"

#include "macros.hpp"

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include <assert.h>

static int gCols;
static int gRows;
static int gLightMapSizeX;
static int gLightMapSizeY;
static float *gLightMapData;
static int gAllocatedSubTextures;
static int gMaxSubLightMaps = 0;

int nextPowerOfTwo(int i)
{
	int j = 1;

	while (j < i)
	{
		j <<= 1;
	}

	return j;
}

int getCntAllocatedSubLightMaps()
{
	return gAllocatedSubTextures;
}

void initAtlas(int cntSubTextures)
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

	gAllocatedSubTextures = 0;
	gMaxSubLightMaps = cntSubTextures;

	PRINT_INT(gLightMapSizeX);
	PRINT_INT(gLightMapSizeY);
	PRINT_INT(cntSubTextures);
}

void destroyAtlas(void)
{
	delete[] gLightMapData;

	gCols = 0;
	gRows = 0;
	gAllocatedSubTextures = 0;
}

float* getSubLightMapPixelPointer(int index, int sx, int sy)
{
	int x = (index % gCols) * LIGHT_MAP_SIZE + sx;
	int y = (index / gCols) * LIGHT_MAP_SIZE + sy;
	return &gLightMapData[y * gLightMapSizeX + x];
}

void setSubLightMapPixel(int index, int sx, int sy, float value)
{
	float *p = getSubLightMapPixelPointer(index, sx, sy);
	*p = value;
}

float getSubLightMapPixel(int index, int sx, int sy)
{
	float *p = getSubLightMapPixelPointer(index, sx, sy);
	return *p;
}

Vector2 transformSubCoords(int index, const Vector2 coords)
{
	return Vector2(((index % gCols) + coords.x) / gCols,
								 ((index / gCols) + coords.y) / gRows);
}

void getAtlasInfo(unsigned int* sizeX, unsigned int* sizeY, const float** data)
{
	*sizeX = gLightMapSizeX;
	*sizeY = gLightMapSizeY;
	*data = gLightMapData;
}
const float* getLightMapData()
{
	return gLightMapData;
}

void getSubLightMap(int index, float data[SIZEOF_LIGHT_MAP])
{
	int i = 0;

	int x;
	int y;

	for (y = 0; y < LIGHT_MAP_SIZE; y++)
	{
		for (x = 0; x < LIGHT_MAP_SIZE; x++)
		{
			data[i++] = getSubLightMapPixel(index, x, y);
		}
	}
}

void setSubLightMap(int index, const float data[SIZEOF_LIGHT_MAP])
{
	int i = 0;

	int x;
	int y;

	for (y = 0; y < LIGHT_MAP_SIZE; y++)
	{
		for (x = 0; x < LIGHT_MAP_SIZE; x++)
		{
			setSubLightMapPixel(index, x, y, data[i++]);
		}
	}
}

/*****/

void allocSubAtlas(SubAtlas* subAtlas)
{
	subAtlas->idxSubLightMap = gAllocatedSubTextures;
	gAllocatedSubTextures++;

	assert(gAllocatedSubTextures <= gMaxSubLightMaps);
}

void setLightMap(const SubAtlas* subAtlas, int x, int y, float value)
{
	assert(x >= 0 && x < LIGHT_MAP_SIZE);
	assert(y >= 0 && y < LIGHT_MAP_SIZE);

	setSubLightMapPixel(subAtlas->idxSubLightMap, x, y, value);
}

void transformCoords(const SubAtlas* subAtlas, Vector2& coords)
{
	if (gCols > 0 && gRows > 0)
	{
		assert(coords.x >= 0.0f && coords.x <= 1.0f);
		assert(coords.y >= 0.0f && coords.y <= 1.0f);

		coords = transformSubCoords(subAtlas->idxSubLightMap, coords);
	}
}
