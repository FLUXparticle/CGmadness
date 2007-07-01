/*
 * CG Madness - a Marble Madness clone
 * Copyright (C) 2007  Sven Reinck
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

#include "lightmap.h"

#include "debug.h"

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include <assert.h>

static int gCols;
static int gRows;
static int gSizeX;
static int gSizeY;
static GLfloat* gData;
static int gAllocatedSubLightMaps;

int nextPowerOfTwo(int i) {
	int j = 1;

	while (j < i) {
		j <<= 1;
	}

	return j;
}

int getCntAllocatedSubLightMaps(void)
{
	return gAllocatedSubLightMaps;
}

void allocLightMap(int cntSubLightMaps) {
	gRows = 1;
	do {
		gRows <<= 1;
		gCols = ((cntSubLightMaps - 1) / gRows) + 1;
	} while (gCols > gRows);

	gCols = nextPowerOfTwo(gCols);

	gSizeX = gCols * LIGHT_MAP_SIZE;
	gSizeY = gRows * LIGHT_MAP_SIZE;

	MALLOC(gData, gSizeX * gSizeY * sizeof(*gData));
	gAllocatedSubLightMaps = 0;

	PRINT_INT(gSizeX);
	PRINT_INT(gSizeY);
	PRINT_INT(cntSubLightMaps);
}

void freeLightMap(void) {
	FREE(gData);
	gAllocatedSubLightMaps = 0;
}

GLfloat* getSubLightMapPixelPointer(int index, int sx, int sy) {
	int x = (index % gCols) * LIGHT_MAP_SIZE + sx;
	int y = (index / gCols) * LIGHT_MAP_SIZE + sy;
	return &gData[y * gSizeX + x];
}

void setSubLightMapPixel(int index, int sx, int sy, GLfloat value) {
	GLfloat* p = getSubLightMapPixelPointer(index, sx, sy);
	*p = value;
}

GLfloat getSubLightMapPixel(int index, int sx, int sy) {
	GLfloat* p = getSubLightMapPixelPointer(index, sx, sy);
	return *p;
}

Vector2 transformSubCoords(int index, const Vector2 coords) {
	return vector2(((index % gCols) + coords.x) / gCols, ((index / gCols) + coords.y) / gRows);
}

void lightMapToTexture(GLuint texID) {
	glBindTexture(GL_TEXTURE_2D, texID);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_LUMINANCE8, gSizeX, gSizeY, 0, GL_LUMINANCE, GL_FLOAT, gData);
}

void getSubLightMap(int index, GLfloat data[SIZEOF_LIGHT_MAP])
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

void setSubLightMap(int index, const GLfloat data[SIZEOF_LIGHT_MAP])
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

void allocSubLightMaps(LightMap* lightMap, int sizeX, int sizeY) {
	lightMap->sizeX = sizeX;
	lightMap->sizeY = sizeY;
	lightMap->idxSubLightMap = gAllocatedSubLightMaps;
	gAllocatedSubLightMaps += sizeX * sizeY;
}

void setLightMap(LightMap* lightMap, int x, int y, GLfloat value) {
	int index;

	assert(x >= 0 && x < lightMap->sizeX * LIGHT_MAP_SIZE);
	assert(y >= 0 && y < lightMap->sizeY * LIGHT_MAP_SIZE);

	index = lightMap->idxSubLightMap + (y / LIGHT_MAP_SIZE) * lightMap->sizeX + (x / LIGHT_MAP_SIZE);
	setSubLightMapPixel(index, x % LIGHT_MAP_SIZE, y % LIGHT_MAP_SIZE, value);
}

Vector2 transformCoords(const LightMap* lightMap, const Vector2 coords) {
	float fx = coords.x * lightMap->sizeX;
	float fy = coords.y * lightMap->sizeY;
	int x = (int) floor(fx);
	int y = (int) floor(fy);
	int index = lightMap->idxSubLightMap + y * lightMap->sizeX + x;
	Vector2 v = vector2(fx - x, fy - y);

	assert(coords.x >= 0.0f && coords.x <= 1.0f);
	assert(coords.y >= 0.0f && coords.y <= 1.0f);

	return transformSubCoords(index, v);
}
