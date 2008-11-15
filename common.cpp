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

static std::vector<SubAtlas> gSubAtlas;

static Orientation orientationFloor(int fx, int fy)
{
	const Square& square = getRoofSquare(fx, fy);

	Orientation floor;

	floor.origin = square.vertices[0];
	floor.vx = square.vertices[1] - square.vertices[0];
	floor.vy = square.vertices[3] - square.vertices[0];
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
		add(Vector3(fx + sgEdgeX[side], fy + sgEdgeY[side], face.bottom),
				sgLevel.origin);
	orientation.vx =
		Vector3(sgEdgeX[next] - sgEdgeX[side], sgEdgeY[next] - sgEdgeY[side], 0.0f);
	orientation.vy = Vector3(0.0f, 0.0f, 1.0f);
	orientation.normal =
		Vector3(sgEdgeX[side] - sgEdgeX[prev], sgEdgeY[side] - sgEdgeY[prev], 0.0f);

	return orientation;
}

Vector2 squareLightmapCoords(const Square& square, int vertex)
{
	static float a = (float) (LIGHT_MAP_SIZE - 1) / LIGHT_MAP_SIZE;
	static float b = 1.0f / (2 * LIGHT_MAP_SIZE);

	SubAtlas* atlas = &gSubAtlas[square.idxAtlas];

	Vector3 decomp = atlas->orientation.decomposition(square.vertices[vertex]);

	Vector2 result;

	result.x = a * decomp.x + b;
	result.y = a * decomp.y + b;

	transformCoords(atlas->idxSubLightMap, result);

	return result;
}

void initCommon()
{
	gSubAtlas.clear();

	for (int x = 0; x < sgLevel.size.x; x++)
	{
		for (int y = 0; y < sgLevel.size.y; y++)
		{
			SubAtlas atlas;
			atlas.orientation = orientationFloor(x, y);
			atlas.idxSubLightMap = gSubAtlas.size();
			gSubAtlas.push_back(atlas);

			Block* b = getBlock(x, y);
			Square* square = &b->roof;
			square->idxAtlas = atlas.idxSubLightMap;

			for (int side = 0; side < 4; side++)
			{
				Orientation orientation = orientationSide(x, y, side);

				unsigned int start = gSubAtlas.size();

				SideFace* face = &b->sideFaces[side];

				FOREACH(face->squares, iter)
				{
					square = &(*iter);

					int z0 = (int) floor(square->vertices[1].z);
					unsigned int i = z0 - face->bottom;

					while (gSubAtlas.size() <= start + i)
					{
						SubAtlas atlas;
						atlas.orientation = orientation;
						atlas.orientation.origin += atlas.orientation.vy
						    * (gSubAtlas.size() - start);
						atlas.idxSubLightMap = gSubAtlas.size();
						gSubAtlas.push_back(atlas);
					}

					square->idxAtlas = start + i;
				}
			}
		}
	}

	initAtlas(gSubAtlas.size());
}

void destroyCommon()
{
	gSubAtlas.clear();

	destroyAtlas();
}

static void updateLightMapIdle(int step)
{
	genAmbientOcclusionTexture(gSubAtlas[step]);
}

void updateLightMap(bool useProgressBar)
{
	int cntSteps = gSubAtlas.size();

	if (useProgressBar)
	{
		startIdle(cntSteps, updateLightMapIdle);
	}
	else
	{
		printf("calculating lightmaps...\n");
		resetProgress();

		for (int step = 0; step < cntSteps;)
		{
			updateLightMapIdle(step);

			step++;
			setProgress((float) step / cntSteps);
		}
	}
}
