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

#include "lightmap.hpp"

#include "level.hpp"

#include "functions.hpp"

#include <math.h>

float approximationSquare(const Vector3 position, const Vector3 normal,
													const Square square)
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

	return 1.0f - ((d1 * d2) / (1.0f + M_PI * sqr(r) / square.area));
}

float approximation(const Vector3 position, const Vector3 normal)
{
	Vector3 z = Vector3(0.0f, 0.0f, 1.0f);
	float light = 1.0f - acos(dot(normal, z)) / M_PI;

	int x;
	int y;

	for (x = 0; x < sgLevel.size.x; x++)
	{
		for (y = 0; y < sgLevel.size.y; y++)
		{
			int check = 0;

			Square square;
			int j;

			if (len
					(sub
					 (position,
						add(Vector3(x + 0.5f, y + 0.5f, position.z),
								sgLevel.origin))) > 6.0f)
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

void genAmbientOcclusionTexture(SubAtlas * lightMap, Orientation orientation)
{
	int x;
	int y;

	for (x = 0; x < lightMap->sizeX * LIGHT_MAP_SIZE; x++)
	{
		for (y = 0; y < lightMap->sizeY * LIGHT_MAP_SIZE; y++)
		{
			float sx =
				(x / LIGHT_MAP_SIZE) + (float) (x % LIGHT_MAP_SIZE) / (LIGHT_MAP_SIZE -
																															 1);
			float sy =
				(y / LIGHT_MAP_SIZE) + (float) (y % LIGHT_MAP_SIZE) / (LIGHT_MAP_SIZE -
																															 1);

			Vector3 off = add(scale(sx, orientation.vx), scale(sy, orientation.vy));
			Vector3 rayPosition = add(orientation.origin, off);

			float light = approximation(rayPosition, orientation.normal);

			setLightMap(lightMap, x, y, light);
		}
	}
}
