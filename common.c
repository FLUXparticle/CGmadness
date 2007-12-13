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

#include "common.h"

#include "atlas.h"
#include "noise.h"
#include "idle.h"
#include "lightmap.h"
#include "level.h"

#include "progress.h"

#include "functions.h"

#include "debug.h"

#include <GL/glut.h>

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <assert.h>

#define BOTTOM -0.0f

typedef struct
{
	SubAtlas sides[4];
} CellLightMap;

static SubAtlas* gSubAtlasFloor;
static CellLightMap* gSubAtlasSides;

#define SUB_ATLAS_FLOOR(_x, _y) (gSubAtlasFloor[(_y) * sgLevel.size.x + (_x)])
#define SUB_ATLAS_SIDES(_x, _y) (gSubAtlasSides[(_y) * sgLevel.size.x + (_x)])

static int quadsNeeded(int fx, int fy, int side)
{
	SideFace face;

	getSideFace(fx, fy, side, &face);

	if (face.cntSquares > 0)
	{
		return (int) (ceil(face.top) - floor(face.bottom));
	}
	else
	{
		return 0;
	}
}

void initCommon(void)
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


	initAtlas(countSubLightMaps);

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

void destroyCommon(void)
{
	FREE(gSubAtlasFloor);
	FREE(gSubAtlasSides);

	destroyAtlas();
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

	orientation.origin = vector3(fx + sgEdgeX[side], fy + sgEdgeY[side], floor(face.bottom));
	orientation.vx = vector3(sgEdgeX[next] - sgEdgeX[side], sgEdgeY[next] - sgEdgeY[side], 0.0f);
	orientation.vy = vector3(0.0f, 0.0f, 1.0f);
	orientation.normal = vector3(sgEdgeX[side] - sgEdgeX[prev], sgEdgeY[side] - sgEdgeY[prev], 0.0f);

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

			genAmbientOcclusionTexture(&SUB_ATLAS_FLOOR(x, y), floor);

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

				genAmbientOcclusionTexture(&SUB_ATLAS_SIDES(x, y).sides[side], orientation);
			}

			steps++;
			setProgress((float) steps / cntSteps);
		}
	}
}

static void calcNoiseIdle(int step)
{
	int side = step % 5;
	int y = step / 5 % sgLevel.size.y;
	int x = step / 5 / sgLevel.size.y;

	if (side < 4)
	{
		Orientation orientation = orientationSide(x, y, side);
		genNoiseTexture(&SUB_ATLAS_SIDES(x, y).sides[side], orientation.origin, orientation.vx, orientation.vy);
	}
	else
	{
		Orientation floor = orientationFloor(x, y);
		genNoiseTexture(&SUB_ATLAS_FLOOR(x, y), floor.origin, floor.vx, floor.vy);
	}
}

void updateColorMap(void)
{
	initNoise();

	startIdle(sgLevel.size.x * sgLevel.size.y * 5, calcNoiseIdle);
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
			
			updatePlate(x, y);

			for (i = 0; i < 4; i++)
			{
				square->texcoord[i].x = sgEdgeX[i] * (int) (len(sub(square->vertices[1], square->vertices[0])) + 0.5f);
				square->texcoord[i].y = sgEdgeY[i] * (int) (len(sub(square->vertices[3], square->vertices[0])) + 0.5f);
				
				if (sgLevel.lightMap)
				{
					square->lightmap[i].x = a * sgEdgeX[i] + b;
					square->lightmap[i].y = a * sgEdgeY[i] + b;
					square->lightmap[i] = transformCoords(&SUB_ATLAS_FLOOR(x, y), square->lightmap[i]);
	
					square->colormap[i].x = c * sgEdgeX[i] + d;
					square->colormap[i].y = c * sgEdgeY[i] + d;
					square->colormap[i] = transformCoords(&SUB_ATLAS_FLOOR(x, y), square->colormap[i]);
				}
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

				int x0 = x + sgEdgeX[side];
				int y0 = y + sgEdgeY[side];

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
						float tx = (sgEdgeX[next] - sgEdgeX[side]) * (square->vertices[i].x - sgLevel.origin.x - x0);
						float ty = (sgEdgeY[next] - sgEdgeY[side]) * (square->vertices[i].y - sgLevel.origin.y - y0);
						float tz = square->vertices[i].z - z0;

						float txy = tx + ty;

						square->texcoord[i].x = txy;
						square->texcoord[i].y = ((1.0f - txy) * z1 + txy * z2) - square->vertices[i].z - sgLevel.origin.z;

						if (sgLevel.lightMap)
						{
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
}
