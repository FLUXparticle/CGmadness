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

#include "common.hpp"

#include "atlas.hpp"
#include "idle.hpp"
#include "lightmap.hpp"
#include "level/level.hpp"

#include "progress.hpp"

#include "functions.hpp"

#include "macros.hpp"

#include GL_H

#include <cstdio>
#include <cmath>

#define BOTTOM -0.0f

struct CellLightMap
{
	int sides[5];
};

static int gCountSubLightMaps;
static SubAtlas* gSubAtlas;

static int quadsNeeded(int fx, int fy, int side)
{
	const SideFace& face = getSideFace(fx, fy, side);

	if (!face.squares.empty())
	{
		return (int) (ceil(face.top) - floor(face.bottom));
	}
	else
	{
		return 0;
	}
}

static Orientation orientationFloor(int fx, int fy)
{
	const Square& square = getRoofSquare(fx, fy);

	Orientation floor;

	floor.origin = square.vertices[0];
	floor.vx = sub(square.vertices[1], square.vertices[0]);
	floor.vy = sub(square.vertices[3], square.vertices[0]);
	floor.normal = square.normal;

	return floor;
}

static Orientation orientationSide(int fx, int fy, int side)
{
	const SideFace& face = getSideFace(fx, fy, side);

	int next = (side + 1) % 4;
	int prev = (side + 3) % 4;

	Orientation orientation;

	orientation.origin =
		add(Vector3(fx + sgEdgeX[side], fy + sgEdgeY[side], floor(face.bottom)),
				sgLevel.origin);
	orientation.vx =
		Vector3(sgEdgeX[next] - sgEdgeX[side], sgEdgeY[next] - sgEdgeY[side], 0.0f);
	orientation.vy = Vector3(0.0f, 0.0f, 1.0f);
	orientation.normal =
		Vector3(sgEdgeX[side] - sgEdgeX[prev], sgEdgeY[side] - sgEdgeY[prev], 0.0f);

	return orientation;
}

void initCommon()
{

	int x;
	int y;
	int side;

	gCountSubLightMaps = 0;

	for (x = 0; x < sgLevel.size.x; x++)
	{
		for (y = 0; y < sgLevel.size.y; y++)
		{
			gCountSubLightMaps++;

			for (side = 0; side < 4; side++)
			{
				gCountSubLightMaps += quadsNeeded(x, y, side);
			}
		}
	}

	gSubAtlas = new SubAtlas[gCountSubLightMaps];

	initAtlas(gCountSubLightMaps);

	int index = 0;
	SubAtlas* atlas;
	for (x = 0; x < sgLevel.size.x; x++)
	{
		for (y = 0; y < sgLevel.size.y; y++)
		{
			atlas = &gSubAtlas[index++];
			allocSubAtlas(atlas);
			atlas->orientation = orientationFloor(x, y);

			Plate* p = getPlate(x, y);
			Square* square = &p->roof;
			square->atlas = atlas;

			for (side = 0; side < 4; side++)
			{
				int quads = quadsNeeded(x, y, side);
				Orientation orientation = orientationSide(x, y, side);

				int start = index;

				for (int i = 0; i < quads; ++i)
				{
					atlas = &gSubAtlas[index++];
					allocSubAtlas(atlas);
					atlas->orientation = orientation;
					orientation.origin += orientation.vy;
				}

				SideFace* face = &p->sideFaces[side];

				FOREACH(face->squares, iter)
				{
					square = &(*iter);

					int z0 = (int) floor(square->vertices[1].z);
					int i = (int) z0 - floor(face->bottom);

					square->atlas = &gSubAtlas[start + i];
				}
			}
		}
	}
}

void destroyCommon()
{
	delete[] gSubAtlas;

	destroyAtlas();
}

static void updateLightMapIdle(int step)
{
	SubAtlas* atlas = &gSubAtlas[step];
	genAmbientOcclusionTexture(atlas, atlas->orientation);
}

void updateLightMap(int useProgressBar)
{
	int cntSteps = gCountSubLightMaps;

	if (useProgressBar)
	{
		startIdle(cntSteps, updateLightMapIdle);
	}
	else
	{
		int step;

		printf("calculating lightmaps...\n");
		resetProgress();

		for (step = 0; step < cntSteps;)
		{
			updateLightMapIdle(step);

			step++;
			setProgress((float) step / cntSteps);
		}
	}
}

void updateTexCoords()
{
	float a = (float) (LIGHT_MAP_SIZE - 1) / LIGHT_MAP_SIZE;
	float b = 1.0f / (2 * LIGHT_MAP_SIZE);

	/*****/

	for (int x = 0; x < sgLevel.size.x; x++)
	{
		for (int y = 0; y < sgLevel.size.y; y++)
		{
			Plate* p = getPlate(x, y);
			Square* square = &p->roof;

			for (int i = 0; i < 4; i++)
			{
				square->texcoord[i].x =
					sgEdgeX[i] *
					(int) (len(sub(square->vertices[1], square->vertices[0])) + 0.5f);
				square->texcoord[i].y =
					sgEdgeY[i] *
					(int) (len(sub(square->vertices[3], square->vertices[0])) + 0.5f);

				if (sgLevel.lightMap)
				{
					square->lightmap[i].x = a * sgEdgeX[i] + b;
					square->lightmap[i].y = a * sgEdgeY[i] + b;
					transformCoords(square->atlas, square->lightmap[i]);
				}
			}
		}
	}

	for (int x = 0; x < sgLevel.size.x; x++)
	{
		for (int y = 0; y < sgLevel.size.y; y++)
		{
			Plate* p = getPlate(x, y);

			for (int side = 0; side < 4; side++)
			{
				int next = (side + 1) % 4;

				int x0 = x + sgEdgeX[side];
				int y0 = y + sgEdgeY[side];

				SideFace* face = &p->sideFaces[side];

				float z1 = p->roof.vertices[side].z;
				float z2 = p->roof.vertices[next].z;

				FOREACH(face->squares, iter)
				{
					Square* square = &(*iter);

					int z0 = (int) square->vertices[1].z;

					for (int i = 0; i < 4; i++)
					{
						float tx =
							(sgEdgeX[next] - sgEdgeX[side]) * (square->vertices[i].x -
																								 sgLevel.origin.x - x0);
						float ty =
							(sgEdgeY[next] - sgEdgeY[side]) * (square->vertices[i].y -
																								 sgLevel.origin.y - y0);
						float tz = square->vertices[i].z - z0;

						float txy = tx + ty;

						square->texcoord[i].x = txy;
						square->texcoord[i].y =
							((1.0f - txy) * z1 + txy * z2) - square->vertices[i].z -
							sgLevel.origin.z;

						if (sgLevel.lightMap)
						{
							square->lightmap[i].x = a * txy + b;
							square->lightmap[i].y = a * tz + b;
							transformCoords(square->atlas, square->lightmap[i]);
						}
					}
				}
			}
		}
	}
}
