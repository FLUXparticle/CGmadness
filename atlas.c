/*
 * CG Madness - a Marble Madness clone
 * Copyright (C) 2007  Sven Reinck <sreinck@gmx.de>
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
 *
 * $Id$
 *
 */

#include "atlas.h"

#include "debug.h"

#include <GL/gl.h>

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include <assert.h>

static int gCols;
static int gRows;
static int gLightMapSizeX;
static int gLightMapSizeY;
static int gColorMapSizeX;
static int gColorMapSizeY;
static float* gLightMapData;
static Color3* gColorMapData;
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

int getCntAllocatedSubLightMaps(void)
{
	return gAllocatedSubTextures;
}

void allocAtlas(int cntSubTextures)
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

	MALLOC(gLightMapData, gLightMapSizeX * gLightMapSizeY * sizeof(*gLightMapData));

	gColorMapSizeX = gCols * COLOR_MAP_SIZE;
	gColorMapSizeY = gRows * COLOR_MAP_SIZE;

	MALLOC(gColorMapData, gColorMapSizeX * gColorMapSizeY * sizeof(*gColorMapData));

#if 1
	{
		int i;
		for (i = 0; i < gLightMapSizeX * gLightMapSizeY; i++)
		{
			gLightMapData[i] = 0.0f;
		}
	}
#endif

	gAllocatedSubTextures = 0;
	gMaxSubLightMaps = cntSubTextures;

	PRINT_INT(gLightMapSizeX);
	PRINT_INT(gLightMapSizeY);
	PRINT_INT(gColorMapSizeX);
	PRINT_INT(gColorMapSizeY);
	PRINT_INT(cntSubTextures);
}

void freeAtlas(void)
{
	FREE(gLightMapData);
	FREE(gColorMapData);

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
	float* p = getSubLightMapPixelPointer(index, sx, sy);
	*p = value;
}

float getSubLightMapPixel(int index, int sx, int sy)
{
	float* p = getSubLightMapPixelPointer(index, sx, sy);
	return *p;
}

void setSubColorMapPixel(int index, int sx, int sy, Color3 col)
{
	int x = (index % gCols) * COLOR_MAP_SIZE + sx;
	int y = (index / gCols) * COLOR_MAP_SIZE + sy;
	gColorMapData[y * gColorMapSizeX + x] = col;
}

Vector2 transformSubCoords(int index, const Vector2 coords)
{
	return vector2(((index % gCols) + coords.x) / gCols, ((index / gCols) + coords.y) / gRows);
}

void lightMapToTexture(unsigned int texID)
{
	glBindTexture(GL_TEXTURE_2D, texID);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_LUMINANCE8, gLightMapSizeX, gLightMapSizeY, 0, GL_LUMINANCE, GL_FLOAT, gLightMapData);
}

void colorMapToTexture(unsigned int texID)
{
	glBindTexture(GL_TEXTURE_2D, texID);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB8, gColorMapSizeX, gColorMapSizeY, 0, GL_RGB, GL_FLOAT, gColorMapData);
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

void allocSubAtlas(SubAtlas* subAtlas, int sizeX, int sizeY)
{
	subAtlas->sizeX = sizeX;
	subAtlas->sizeY = sizeY;
	subAtlas->idxSubLightMap = gAllocatedSubTextures;
	gAllocatedSubTextures += sizeX * sizeY;

	assert(gAllocatedSubTextures <= gMaxSubLightMaps);
}

void setLightMap(SubAtlas* subAtlas, int x, int y, float value)
{
	int index;

	assert(x >= 0 && x < subAtlas->sizeX * LIGHT_MAP_SIZE);
	assert(y >= 0 && y < subAtlas->sizeY * LIGHT_MAP_SIZE);

	index = subAtlas->idxSubLightMap + (y / LIGHT_MAP_SIZE) * subAtlas->sizeX + (x / LIGHT_MAP_SIZE);
	setSubLightMapPixel(index, x % LIGHT_MAP_SIZE, y % LIGHT_MAP_SIZE, value);
}

void setColorMap(SubAtlas* subAtlas, int x, int y, Color3 col)
{
	int index;

	assert(x >= 0 && x < subAtlas->sizeX * COLOR_MAP_SIZE);
	assert(y >= 0 && y < subAtlas->sizeY * COLOR_MAP_SIZE);

	index = subAtlas->idxSubLightMap + (y / COLOR_MAP_SIZE) * subAtlas->sizeX + (x / COLOR_MAP_SIZE);
	setSubColorMapPixel(index, x % COLOR_MAP_SIZE, y % COLOR_MAP_SIZE, col);
}

Vector2 transformCoords(const SubAtlas* subAtlas, const Vector2 coords)
{
	if (gCols > 0 && gRows > 0)
	{
		float fx = coords.x;
		float fy = coords.y;
		int x = (int) floor(fx);
		int y = (int) floor(fy);
		int index = subAtlas->idxSubLightMap + y * subAtlas->sizeX + x;
		Vector2 v = vector2(fx - x, fy - y);

		assert(coords.x >= 0.0f && coords.x <= subAtlas->sizeX);
		assert(coords.y >= 0.0f && coords.y <= subAtlas->sizeY);

		return transformSubCoords(index, v);
	}
	else
	{
		return coords;
	}
}
