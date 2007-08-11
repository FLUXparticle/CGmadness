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

#include "common.h"

#include "atlas.h"
#include "noise.h"
#include "progress.h"
#include "crc32.h"

#include "functions.h"

#include "debug.h"

#include <GL/glu.h>

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <assert.h>

#if (MOUSE_CONTROL)
# define CAMERA_MOVE_TIME_CONSTANT 100.0f
#else
# define CAMERA_MOVE_TIME_CONSTANT 5.0f
#endif

#define THIS_CGM_VERSION 2

#define BOTTOM -0.0f

typedef struct
{
	SubAtlas sides[4];
} CellLightMap;

typedef struct
{
	Vector3 origin;
	Vector3 vx;
	Vector3 vy;
	Vector3 normal;
} Orientation;

int sgCntVertices;

int sgMaxPlates;
int sgMaxQuads;
int sgMaxVertices;

Vector3* sgVertices;
Vector3* sgNormals;

Level sgLevel;

Vector3 sgForward;
Vector3 sgRight;

static const int gEdgeX[4] = { 0, 1, 1, 0 };
static const int gEdgeY[4] = { 0, 0, 1, 1 };

static SubAtlas* gSubAtlasFloor;
static CellLightMap* gSubAtlasSides;

#define SUB_ATLAS_FLOOR(x, y) (gSubAtlasFloor[(y) * sgLevel.size.x + (x)])
#define SUB_ATLAS_SIDES(x, y) (gSubAtlasSides[(y) * sgLevel.size.x + (x)])

float getMinZValue(const Square* square) {
	int i;
	float res = square->vertices[0].z;
	for (i = 1 ; i < 4; i++) {
		if (square->vertices[i].z < res) {
			res = square->vertices[i].z;
		}
	}
	return res;
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

int getFieldEdgeHeight(int x, int y, int edge)
{
	if (x >= 0 && y >= 0 && x < sgLevel.size.x && y < sgLevel.size.y)
	{
		Plate* p = &sgLevel.field[x][y];

		return 5 + p->z + (gEdgeX[edge] * 2 - 1) * p->dzx + (gEdgeY[edge] * 2 - 1) * p->dzy;
	}

	/* value out of range */
	return 0;
}

void updateSquareAttributes(Square* square)
{
	square->mid = midpoint(square->vertices);

	square->area = 0.5f * len(cross(sub(square->vertices[1], square->vertices[0]), sub(square->vertices[3], square->vertices[0]))) +
				         0.5f * len(cross(sub(square->vertices[1], square->vertices[2]), sub(square->vertices[3], square->vertices[2])));
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
			square->vertices[i].x = x + gEdgeX[i];
			square->vertices[i].y = y + gEdgeY[i];
			square->vertices[i].z = (float) getFieldEdgeHeight(x, y, i) / HEIGHT_STEPS;
		}

		updateSquareAttributes(square);

		for (side = 0; side < 4; side++)
		{
			int next = (side + 1) % 4;
			int prev = (side + 3) % 4;

			int sideOpposite = (side + 3) % 4;
			int nextOpposite = (side + 2) % 4;

			int dx = gEdgeX[side] - gEdgeX[prev];
			int dy = gEdgeY[side] - gEdgeY[prev];

			int height1 = getFieldEdgeHeight(x, y, side);
			int height2 = getFieldEdgeHeight(x, y, next);

			int height3 = getFieldEdgeHeight(x + dx, y + dy, sideOpposite);
			int height4 = getFieldEdgeHeight(x + dx, y + dy, nextOpposite);

			SideFace* face = &p->sideFaces[side];

			face->cntSquares = 0;

			if (height1 > height3 ||
					height2 > height4)
			{
				int x1 = x + gEdgeX[side];
				int y1 = y + gEdgeY[side];
				int x2 = x + gEdgeX[next];
				int y2 = y + gEdgeY[next];

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

int quadsNeeded(int fx, int fy, int side)
{
	SideFace face;

	getSideFace(fx, fy, side, &face);

	return (int) (ceil(face.top) - floor(face.bottom));
}

void initAtlas(void)
{
	int countSubLightMaps = 0;

	int x;
	int y;
	int side;

	MALLOC(gSubAtlasFloor, sgLevel.size.x * sgLevel.size.y * sizeof(*gSubAtlasFloor));

	MALLOC(gSubAtlasSides, sgLevel.size.x * sgLevel.size.y * sizeof(*gSubAtlasSides));

	countSubLightMaps += sgLevel.size.x * sgLevel.size.y;

	for (x = 0; x < sgLevel.size.x; x++)
	{
		for (y = 0; y < sgLevel.size.y; y++)
		{
			for (side = 0; side < 4; side++)
			{
				countSubLightMaps += quadsNeeded(x, y, side);
			}
		}
	}


	allocAtlas(countSubLightMaps);

	for (x = 0; x < sgLevel.size.x; x++)
	{
		for (y = 0; y < sgLevel.size.y; y++)
		{
			allocSubAtlas(&SUB_ATLAS_FLOOR(x, y), 1, 1);

			for (side = 0; side < 4; side++)
			{
				allocSubAtlas(&SUB_ATLAS_SIDES(x, y).sides[side], 1, quadsNeeded(x, y, side));
			}
		}
	}
}

float approximationSquare(const Vector3 position, const Vector3 normal, const Square square)
{
	Vector3 d;
	float r;
	float d1;
	float d2;

	d = sub(square.mid, position);

	r = len(d);

	d = scale(1.0f / r, d);

	d1 = dot(d, normal);
	d2 = dot(d, square.normal);

	if (d1 <= 0.0f || d2 <= 0.0f)
	{
		return 1.0f;
	}

	return 1.0f - ((d1 * d2) / (1.0f + PI * sqr(r) / square.area));
}

float approximation(const Vector3 position, const Vector3 normal)
{
	Vector3 z = vector3(0.0f, 0.0f, 1.0f);
	float light = 1.0f - acos(dot(normal, z)) / PI;

	int x;
	int y;

	for (x = 0; x < sgLevel.size.x; x++)
	{
		for (y = 0; y < sgLevel.size.y; y++)
		{
			int check = 0;

			Square square;
			int j;

			if (len(sub(position, vector3(x + 0.5f, y + 0.5f, position.z))) > 6.0f)
			{
				continue;
			}


			getRoofSquare(x, y, &square);

			for (j = 0; j < 4 && !check; j++)
			{
				if (dot(sub(square.vertices[j], position), normal) > 0.0f)
				{
					check = 1;
				}
			}

			if (!check)
			{
				continue;
			}

			light *= approximationSquare(position, normal, square);

			for (j = 0; j < 4; j++)
			{
				SideFace face;
				int k;

				getSideFace(x, y, j, &face);

				for (k = 0; k < face.cntSquares; k++)
				{
					light *= approximationSquare(position, normal, face.squares[k]);
				}
			}
		}
	}

	return light;
}

void makeAmbientOcclusionTexture(SubAtlas* lightMap, Orientation orientation)
{
	int x;
	int y;

	for (x = 0; x < lightMap->sizeX * LIGHT_MAP_SIZE; x++)
	{
		for (y = 0; y < lightMap->sizeY * LIGHT_MAP_SIZE; y++)
		{
			float sx = (x / LIGHT_MAP_SIZE) + (float) (x % LIGHT_MAP_SIZE) / (LIGHT_MAP_SIZE - 1);
			float sy = (y / LIGHT_MAP_SIZE) + (float) (y % LIGHT_MAP_SIZE) / (LIGHT_MAP_SIZE - 1);

			Vector3 off = add(scale(sx, orientation.vx), scale(sy, orientation.vy));
			Vector3 rayPosition = add(orientation.origin, off);

			float light = approximation(rayPosition, orientation.normal);

			setLightMap(lightMap, x, y, light);
		}
	}
}

Orientation orientationFloor(int fx, int fy)
{
	Square square;
	Orientation floor;

	getRoofSquare(fx, fy, &square);

	floor.origin = square.vertices[0];
	floor.vx = sub(square.vertices[1], square.vertices[0]);
	floor.vy = sub(square.vertices[3], square.vertices[0]);
	floor.normal = square.normal;

	return floor;
}

Orientation orientationSide(int fx, int fy, int side)
{
	int next = (side + 1) % 4;
	int prev = (side + 3) % 4;

	SideFace face;

	Orientation orientation;

	getSideFace(fx, fy, side, &face);

	orientation.origin = vector3(fx + gEdgeX[side], fy + gEdgeY[side], floor(face.bottom));
	orientation.vx = vector3(gEdgeX[next] - gEdgeX[side], gEdgeY[next] - gEdgeY[side], 0.0f);
	orientation.vy = vector3(0.0f, 0.0f, 1.0f);
	orientation.normal = vector3(gEdgeX[side] - gEdgeX[prev], gEdgeY[side] - gEdgeY[prev], 0.0f);

	return orientation;
}

void updateLightMap(void)
{
	int x;
	int y;
	int side;

	int steps = 0;
	int cntSteps = 2 * sgLevel.size.x * sgLevel.size.y;

	printf("calculating lightmaps...\n");
	resetProgress();

	/*****/

	for (x = 0; x < sgLevel.size.x; x++)
	{
		for (y = 0; y < sgLevel.size.y; y++)
		{
			Orientation floor = orientationFloor(x, y);

			makeAmbientOcclusionTexture(&SUB_ATLAS_FLOOR(x, y), floor);

			steps++;
			setProgress((float) steps / cntSteps);
		}
	}

	for (x = 0; x < sgLevel.size.x; x++)
	{
		for (y = 0; y < sgLevel.size.y; y++)
		{
			for (side = 0; side < 4; side++)
			{
				Orientation orientation = orientationSide(x, y, side);

				makeAmbientOcclusionTexture(&SUB_ATLAS_SIDES(x, y).sides[side], orientation);
			}

			steps++;
			setProgress((float) steps / cntSteps);
		}
	}
}

void updateColorMap(void)
{
	int x;
	int y;
	int side;

	initNoise();

	/*****/

	for (x = 0; x < sgLevel.size.x; x++)
	{
		for (y = 0; y < sgLevel.size.y; y++)
		{
			Orientation floor = orientationFloor(x, y);

			genNoiseTexture(&SUB_ATLAS_FLOOR(x, y), floor.origin, floor.vx, floor.vy);
		}
	}

	for (x = 0; x < sgLevel.size.x; x++)
	{
		for (y = 0; y < sgLevel.size.y; y++)
		{
			for (side = 0; side < 4; side++)
			{
				Orientation orientation = orientationSide(x, y, side);

				genNoiseTexture(&SUB_ATLAS_SIDES(x, y).sides[side], orientation.origin, orientation.vx, orientation.vy);
			}
		}
	}
}

void updateTexCoords(void)
{
	int x;
	int y;
	int side;

	float a = (float) (LIGHT_MAP_SIZE - 1) / LIGHT_MAP_SIZE;
	float b = 1.0f / (2 * LIGHT_MAP_SIZE);
	float c = (float) (COLOR_MAP_SIZE - 1) / COLOR_MAP_SIZE;
	float d = 1.0f / (2 * COLOR_MAP_SIZE);

	/*****/

	for (x = 0; x < sgLevel.size.x; x++)
	{
		for (y = 0; y < sgLevel.size.y; y++)
		{
			Plate* p = &sgLevel.field[x][y];
			Square* square = &p->roof;

			int i;

			for (i = 0; i < 4; i++)
			{
				square->texcoord[i].x = gEdgeX[i];
				square->texcoord[i].y = gEdgeY[i];

				square->lightmap[i].x = a * gEdgeX[i] + b;
				square->lightmap[i].y = a * gEdgeY[i] + b;
				square->lightmap[i] = transformCoords(&SUB_ATLAS_FLOOR(x, y), square->lightmap[i]);

				square->colormap[i].x = c * gEdgeX[i] + d;
				square->colormap[i].y = c * gEdgeY[i] + d;
				square->colormap[i] = transformCoords(&SUB_ATLAS_FLOOR(x, y), square->colormap[i]);
			}
		}
	}

	for (x = 0; x < sgLevel.size.x; x++)
	{
		for (y = 0; y < sgLevel.size.y; y++)
		{
			Plate* p = &sgLevel.field[x][y];

			for (side = 0; side < 4; side++)
			{
				int next = (side + 1) % 4;

				int x0 = x + gEdgeX[side];
				int y0 = y + gEdgeY[side];

				SideFace* face = &p->sideFaces[side];

				int k = face->cntSquares - 1;

				float z1 = p->roof.vertices[side].z;
				float z2 = p->roof.vertices[next].z;

				for (k = 0; k < face->cntSquares; k++)
				{
					Square* square = &face->squares[k];

					int z0 = (int) square->vertices[1].z;

					int i;
					for (i = 0; i < 4; i++)
					{
						float tx = (gEdgeX[next] - gEdgeX[side]) * (square->vertices[i].x - x0);
						float ty = (gEdgeY[next] - gEdgeY[side]) * (square->vertices[i].y - y0);
						float tz = square->vertices[i].z - z0;

						float txy = tx + ty;

						square->texcoord[i].x = txy;
						square->texcoord[i].y = ((1.0f - txy) * z1 + txy * z2) - square->vertices[i].z;

						square->lightmap[i].x = a * txy + b;
						square->lightmap[i].y = z0 + a * tz + b - floor(face->bottom);
						square->lightmap[i] = transformCoords(&SUB_ATLAS_SIDES(x, y).sides[side], square->lightmap[i]);

						square->colormap[i].x = c * txy + d;
						square->colormap[i].y = z0 + c * tz + d - floor(face->bottom);
						square->colormap[i] = transformCoords(&SUB_ATLAS_SIDES(x, y).sides[side], square->colormap[i]);
					}
				}
			}
		}
	}
}

void destroyCommon(void)
{
	FREE(gSubAtlasSides);

	freeAtlas();

	FREE(sgLevel.field[0]);
	FREE(sgLevel.field);
	FREE(sgVertices);
	FREE(sgNormals);

	sgLevel.size.x = -1;
	sgLevel.size.y = -1;
}

void allocLevelDataMemory(void)
{
	int x;

	MALLOC(sgLevel.field, sgLevel.size.x * sizeof(Plate*));
	MALLOC(sgLevel.field[0], sgLevel.size.x * sgLevel.size.y * sizeof(Plate));
	for (x = 1; x < sgLevel.size.x; x++)
	{
		sgLevel.field[x] = &sgLevel.field[x - 1][sgLevel.size.y];
	}

  /* init level stuff */
	sgMaxPlates = sgLevel.size.x * sgLevel.size.y;
	sgMaxQuads = (1 + 4 * (MAX_LEVEL_HEIGHT + 1)) * sgMaxPlates;
	sgMaxVertices = 4 * sgMaxQuads;

	sgCntVertices = 0;
	MALLOC(sgVertices, sgMaxVertices * sizeof(Vector3));
	MALLOC(sgNormals, sgMaxVertices * sizeof(Vector3));
}

void newLevel(void)
{
	int x, y;

	fprintf(stderr, "creating new level: (%d, %d)\n", sgLevel.size.x, sgLevel.size.y);

	sgLevel.start.x = 0;
	sgLevel.start.y = 0;

	sgLevel.finish.x = sgLevel.size.x - 1;
	sgLevel.finish.y = sgLevel.size.y - 1;

	allocLevelDataMemory();

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

int loadFieldFromFile(const char* filename)
{
	FILE* file = fopen(filename, "rt");
	int result = 1;

	int x, y;
	FieldCoord fileCoords;
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
	readFieldCoord(file, &fileCoords);

	/* read size from file, if not given through program parameters */
	if (sgLevel.size.x < 0 || sgLevel.size.y < 0)
	{
		sgLevel.size = fileCoords;
	}

	allocLevelDataMemory();

	/* reading data */
	for (x = 0; x < sgLevel.size.x; x++)
	{
		for (y = 0; y < sgLevel.size.y; y++)
		{
			Plate* p = &sgLevel.field[x][y];
			if (x < fileCoords.x && y < fileCoords.y)
			{
				readFieldPlate(file, p);
			}
			else
			{ /* growing */
				p->z = 0;
				p->dzx = 0;
				p->dzy = 0;
			}

			p->dirty = 1;
		}

		/* shrinking */
		if (fileCoords.y > sgLevel.size.y)
		{
			Plate dummyPlate;

			for (y = sgLevel.size.y; y < fileCoords.y; y++)
			{
				readFieldPlate(file, &dummyPlate);
			}
		}
	}

	initAtlas();

	if (version >= 2)
	{
		fscanf(file, "%x\n", &crc32);

		if (crc32 != getCRC32())
		{
			fprintf(stderr, "checksum mismatch: %s\n", filename);
			result = 0;
		}
		else
		{
			int cntSubLightMaps = getCntAllocatedSubLightMaps();

			int i;
			int j;

			for (i = 0; i < cntSubLightMaps; i++)
			{
				GLfloat data[SIZEOF_LIGHT_MAP];

				for (j = 0; j < SIZEOF_LIGHT_MAP; j++)
				{
					int value = readInt(file);
					data[j] = (float) value / 255;
				}

				setSubLightMap(i, data);
			}

			fscanf(file, "%x\n", &crc32);

			if (crc32 != getCRC32())
			{
				fprintf(stderr, "checksum mismatch: %s\n", filename);
				result = 0;
			}
		}
	}
	else
	{
		updateLightMap();
	}

	fclose(file);

	updateTexCoords();

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
			GLfloat data[SIZEOF_LIGHT_MAP];

			getSubLightMap(i, data);

			for (j = 0; j < SIZEOF_LIGHT_MAP; j++)
			{
				writeInt(file, (int) (clamp(data[j], 0.0f, 1.0f) * 255));
				fputc(' ', file);
			}
			fputc('\n', file);
		}

		fprintf(file, "%08X\n", getCRC32());
	}

	if (fclose(file) != 0) {
		return 0;
	}

	return 1;
}
