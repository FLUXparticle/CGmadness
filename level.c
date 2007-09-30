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
 */

#include "level.h"

#include "common.h"
#include "atlas.h"
#include "crc32.h"

#include "functions.h"
#include "debug.h"

#include <GL/gl.h>

#include <stdio.h>
#include <stdlib.h>

#define THIS_CGM_VERSION 3

const int sgEdgeX[4] = { 0, 1, 1, 0 };
const int sgEdgeY[4] = { 0, 0, 1, 1 };

Level sgLevel;

static void updateSquareAttributes(Square* square)
{
	square->mid = midpoint(square->vertices);

	square->area = 0.5f * len(cross(sub(square->vertices[1], square->vertices[0]), sub(square->vertices[3], square->vertices[0]))) +
				         0.5f * len(cross(sub(square->vertices[1], square->vertices[2]), sub(square->vertices[3], square->vertices[2])));
}

static int getFieldEdgeHeight(int x, int y, int edge)
{
	if (x >= 0 && y >= 0 && x < sgLevel.size.x && y < sgLevel.size.y)
	{
		Plate* p = &sgLevel.field[x][y];

		return 5 + p->z + (sgEdgeX[edge] * 2 - 1) * p->dzx + (sgEdgeY[edge] * 2 - 1) * p->dzy;
	}

	/* value out of range */
	return 0;
}

void updatePlate(int x, int y)
{
	Plate* p = &sgLevel.field[x][y];

	if (p->dirty)
	{
		Square* square = &p->roof;
		int i;
		int side;
		int dzx = p->dzx;
		int	dzy = p->dzy;

		Vector3 ex;
		Vector3 ey;

		ex.x = 0.5f;
		ex.y = 0.0f;
		ex.z = (float) dzx / HEIGHT_STEPS;

		ey.x = 0.0f;
		ey.y = 0.5f;
		ey.z = (float) dzy / HEIGHT_STEPS;

		square->normal = norm(cross(ex, ey));

		for (i = 0; i < 4; i++)
		{
			square->vertices[i].x = x + sgEdgeX[i];
			square->vertices[i].y = y + sgEdgeY[i];
			square->vertices[i].z = (float) getFieldEdgeHeight(x, y, i) / HEIGHT_STEPS;
		}

		updateSquareAttributes(square);

		for (side = 0; side < 4; side++)
		{
			int next = (side + 1) % 4;
			int prev = (side + 3) % 4;

			int sideOpposite = (side + 3) % 4;
			int nextOpposite = (side + 2) % 4;

			int dx = sgEdgeX[side] - sgEdgeX[prev];
			int dy = sgEdgeY[side] - sgEdgeY[prev];

			int height1 = getFieldEdgeHeight(x, y, side);
			int height2 = getFieldEdgeHeight(x, y, next);

			int height3 = getFieldEdgeHeight(x + dx, y + dy, sideOpposite);
			int height4 = getFieldEdgeHeight(x + dx, y + dy, nextOpposite);

			SideFace* face = &p->sideFaces[side];

			face->cntSquares = 0;

			if (height1 > height3 ||
					height2 > height4)
			{
				int x1 = x + sgEdgeX[side];
				int y1 = y + sgEdgeY[side];
				int x2 = x + sgEdgeX[next];
				int y2 = y + sgEdgeY[next];

				int startHeight = min(height3, height4);

				int minHeight;
				int maxHeight;
				int x3;
				int y3;

				int bottom;

				if (height1 > height2)
				{
					minHeight = height2;
					maxHeight = height1;
					x3 = x1;
					y3 = y1;
				}
				else
				{
					minHeight = height1;
					maxHeight = height2;
					x3 = x2;
					y3 = y2;
				}

				face->bottom = (float) startHeight / HEIGHT_STEPS;
				face->top    = (float)   maxHeight / HEIGHT_STEPS;

				for (bottom = startHeight; bottom < minHeight; )
				{
					int top = min(minHeight, bottom - (bottom % HEIGHT_STEPS) + HEIGHT_STEPS);

					square = &face->squares[face->cntSquares++];

					square->normal.x = dx;
					square->normal.y = dy;
					square->normal.z = 0.0f;

					square->vertices[0].x = x1;
					square->vertices[0].y = y1;
					square->vertices[0].z = (float) top / HEIGHT_STEPS;

					square->vertices[1].x = x1;
					square->vertices[1].y = y1;
					square->vertices[1].z = (float) bottom / HEIGHT_STEPS;

					square->vertices[2].x = x2;
					square->vertices[2].y = y2;
					square->vertices[2].z = (float) bottom / HEIGHT_STEPS;

					square->vertices[3].x = x2;
					square->vertices[3].y = y2;
					square->vertices[3].z = (float) top / HEIGHT_STEPS;

					updateSquareAttributes(square);

					bottom = top;
				}

				for (; bottom < maxHeight; )
				{
					int top = min(maxHeight, bottom - (bottom % HEIGHT_STEPS) + HEIGHT_STEPS);

					float t1 = (float) (bottom - minHeight) / (maxHeight - minHeight);
					float t2 = (float) (top - minHeight) / (maxHeight - minHeight);

					square = &face->squares[face->cntSquares++];

					square->normal.x = dx;
					square->normal.y = dy;
					square->normal.z = 0.0f;

					square->vertices[0].x = x1 * (1.0f - t2) + x3 * t2;
					square->vertices[0].y = y1 * (1.0f - t2) + y3 * t2;
					square->vertices[0].z = (float) top / HEIGHT_STEPS;

					square->vertices[1].x = x1 * (1.0f - t1) + x3 * t1;
					square->vertices[1].y = y1 * (1.0f - t1) + y3 * t1;
					square->vertices[1].z = (float) bottom / HEIGHT_STEPS;

					square->vertices[2].x = x2 * (1.0f - t1) + x3 * t1;
					square->vertices[2].y = y2 * (1.0f - t1) + y3 * t1;
					square->vertices[2].z = (float) bottom / HEIGHT_STEPS;

					square->vertices[3].x = x2 * (1.0f - t2) + x3 * t2;
					square->vertices[3].y = y2 * (1.0f - t2) + y3 * t2;
					square->vertices[3].z = (float) top / HEIGHT_STEPS;

					updateSquareAttributes(square);

					bottom = top;
				}
			}

		}

		p->dirty = 0;
	}
}

float getMaxZValue(const Square* square) {
	int i;
	float res = square->vertices[0].z;
	for (i = 1 ; i < 4; i++) {
		if (square->vertices[i].z > res) {
			res = square->vertices[i].z;
		}
	}
	return res;
}

void initLevel(void)
{
	int x;

	MALLOC(sgLevel.field, sgLevel.size.x * sizeof(Plate*));
	MALLOC(sgLevel.field[0], sgLevel.size.x * sgLevel.size.y * sizeof(Plate));
	for (x = 1; x < sgLevel.size.x; x++)
	{
		sgLevel.field[x] = &sgLevel.field[x - 1][sgLevel.size.y];
	}
}

void destroyLevel(void) {
	FREE(sgLevel.field[0]);
	FREE(sgLevel.field);
	
	sgLevel.size.x = -1;
	sgLevel.size.y = -1;

	destroyCommon();
}

void getRoofSquare(int x, int y, Square* square)
{
	Plate* p = &sgLevel.field[x][y];
	updatePlate(x, y);
	*square = p->roof;
}

void getSideFace(int x, int y, int side, SideFace* face)
{
	Plate* p = &sgLevel.field[x][y];
	updatePlate(x, y);

	*face = p->sideFaces[side];
}

void newLevel(void)
{
	int x, y;

	fprintf(stderr, "creating new level: (%d, %d)\n", sgLevel.size.x, sgLevel.size.y);

	sgLevel.start.x = 0;
	sgLevel.start.y = 0;

	sgLevel.finish.x = sgLevel.size.x - 1;
	sgLevel.finish.y = sgLevel.size.y - 1;

	initLevel();

	for (x = 0; x < sgLevel.size.x; x++)
	{
		for (y = 0; y < sgLevel.size.y; y++)
		{
			Plate* p = &sgLevel.field[x][y];
			p->z = 0;
			p->dzx = 0;
			p->dzy = 0;
		}
	}
}

int readInt(FILE* file)
{
	int value;

	fscanf(file, "%i", &value);
	nextByte(value);

	return value;
}

void readFieldCoord(FILE* file, FieldCoord* coord)
{
	coord->x = readInt(file);
	coord->y = readInt(file);
}

void readFieldPlate(FILE* file, Plate* plate)
{
	plate->z = readInt(file);
	plate->dzx = readInt(file);
	plate->dzy = readInt(file);
}

void readRLEInt(FILE* file, int* repeat, int* value) {
	int i;
	
	if (fscanf(file, "%ix%i", repeat, value) < 2)
	{
		*value = *repeat;
		*repeat = 1;
	}

	for (i = 0; i < *repeat; i++)
	{
		nextByte(*value);
	}
}

void readRLE(FILE* file, int data[SIZEOF_LIGHT_MAP])
{
	int s = 0;
	int i;
	
	while (s < SIZEOF_LIGHT_MAP)
	{
		int repeat;
		int value;
		
		readRLEInt(file, &repeat, &value);
		
		for (i = 0; i < repeat; i++)
		{
			data[s + i] = value;
		}
		
		s += repeat;
	}
}

int loadFieldFromFile(const char* filename)
{
	FILE* file = fopen(filename, "rt");
	int result = 1;

	int x, y;
	unsigned int version;
	unsigned int crc32;

	if (!file)
	{
		fprintf(stderr, "can not open file: %s\n", filename);
		return 0;
	}

	/* version number */
	fscanf(file, "v%u", &version);

	if (version > THIS_CGM_VERSION)
	{
		fprintf(stderr, "incompatible version number: %u\n", version);
		return 0;
	}

	resetCRC32();

	/* read attributes */
	readFieldCoord(file, &sgLevel.start);
	readFieldCoord(file, &sgLevel.finish);
	readFieldCoord(file, &sgLevel.size);

	initLevel();

	/* reading data */
	for (x = 0; x < sgLevel.size.x; x++)
	{
		for (y = 0; y < sgLevel.size.y; y++)
		{
			Plate* p = &sgLevel.field[x][y];
			readFieldPlate(file, p);

			p->dirty = 1;
		}
	}

	initCommon();

	if (version >= 2)
	{
		fscanf(file, "%x\n", &crc32);

		if (crc32 != getCRC32())
		{
			fprintf(stderr, "1st checksum mismatch: %s\n", filename);
			result = 0;
		}
		else
		{
			int cntSubLightMaps = getCntAllocatedSubLightMaps();

			int i;
			int j;

			for (i = 0; i < cntSubLightMaps; i++)
			{
				int dataInt[SIZEOF_LIGHT_MAP];
				GLfloat dataFloat[SIZEOF_LIGHT_MAP];
				
				readRLE(file, dataInt);

				for (j = 0; j < SIZEOF_LIGHT_MAP; j++)
				{
					dataFloat[j] = (float) dataInt[j] / 255;
				}

				setSubLightMap(i, dataFloat);
			}

			fscanf(file, "%x\n", &crc32);

			if (crc32 != getCRC32())
			{
				fprintf(stderr, "2st checksum mismatch: %s\n", filename);
				result = 0;
			}
		}
	}
	else
	{
		updateLightMap();
	}

	fclose(file);

	return result;
}

void writeInt(FILE* file, int value) {
	fprintf(file, "%i", value);
	nextByte(value);
}

void writeFieldCoord(FILE* file, const FieldCoord coord) {
	writeInt(file, coord.x);
	fputc(' ', file);
	writeInt(file, coord.y);
	fputc('\n', file);
}

void writeFieldPlate(FILE* file, const Plate* plate) {
	writeInt(file, plate->z);
	fputc(' ', file);
	writeInt(file, plate->dzx);
	fputc(' ', file);
	writeInt(file, plate->dzy);
	fputc('\n', file);
}

void writeRLEInt(FILE* file, int repeat, int value) {
	int i;
	
	if (repeat > 1)
	{
		fprintf(file, "%ix%i", repeat, value);
	}
	else
	{
		fprintf(file, "%i", value);
	}
	
	for (i = 0; i < repeat; i++)
	{
		nextByte(value);
	}
}

void writeRLE(FILE* file, const int data[SIZEOF_LIGHT_MAP])
{
	int s = 0;
	int l = 1;
	
	while (s < SIZEOF_LIGHT_MAP)
	{
		if (s + l < SIZEOF_LIGHT_MAP && data[s] == data[s + l])
		{
			l++;
		}
		else
		{
			writeRLEInt(file, l, data[s]);
			
			fputc(' ', file);
			s += l;
			l = 1;
		}
	}
	fputc('\n', file);
}

int saveFieldToFile(const char* filename) {
	FILE* file = fopen(filename, "wt");
	int x, y;

	if (!file) return 0;

	/* version number */
	fprintf(file, "v%u\n", THIS_CGM_VERSION);

	resetCRC32();

	/* write atributes */
	writeFieldCoord(file, sgLevel.start);
	writeFieldCoord(file, sgLevel.finish);
	writeFieldCoord(file, sgLevel.size);

	/* write data */
	for (x = 0; x < sgLevel.size.x; x++) {
		for (y = 0; y < sgLevel.size.y; y++) {
			Plate* p = &sgLevel.field[x][y];
			writeFieldPlate(file, p);
		}
	}

	fprintf(file, "%08X\n", getCRC32());

	{
		int cntSubLightMaps = getCntAllocatedSubLightMaps();

		int i;
		int j;

		for (i = 0; i < cntSubLightMaps; i++)
		{
			GLfloat dataFloat[SIZEOF_LIGHT_MAP];
			int dataInt[SIZEOF_LIGHT_MAP];

			getSubLightMap(i, dataFloat);

			for (j = 0; j < SIZEOF_LIGHT_MAP; j++)
			{
				dataInt[j] = (int) (clamp(dataFloat[j], 0.0f, 1.0f) * 255);
			}
			
			writeRLE(file, dataInt);
		}

		fprintf(file, "%08X\n", getCRC32());
	}

	if (fclose(file) != 0) {
		return 0;
	}

	return 1;
}
