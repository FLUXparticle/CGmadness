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

#include "level/level.hpp"
#include "atlas.hpp"

#include "functions.hpp"

#include "macros.hpp"

#include GL_H

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

float approximation(const Vector3& position, const Vector3& normal)
{
	Vector3 z(0.0f, 0.0f, 1.0f);
	float light = 1.0f - acos(dot(normal, z)) / M_PI;

	for (int x = 0; x < sgLevel.size.x; x++)
	{
		for (int y = 0; y < sgLevel.size.y; y++)
		{
			if ((position - sgLevel.origin - Vector3(x + 0.5f, y + 0.5f, position.z)).len()
			    > 6.0f)
			{
				continue;
			}

			bool check = false;
			const Square& square = getRoofSquare(x, y);

			for (int j = 0; j < 4; j++)
			{
				if (dot(square.vertices[j] - position, normal) > 0.0f)
				{
					check = true;
					break;
				}
			}

			if (!check)
			{
				continue;
			}

			light *= approximationSquare(position, normal, square);

			for (int j = 0; j < 4; j++)
			{
				const SideFace& face = getSideFace(x, y, j);

				FOREACH(face.squares, iter)
				{
					light *= approximationSquare(position, normal, *iter);
				}
			}
		}
	}

	return light;
}

void genAmbientOcclusionTexture(const SubAtlas* lightMap)
{
	const Orientation& orientation = lightMap->orientation;
	for (int x = 0; x < LIGHT_MAP_SIZE; x++)
	{
		for (int y = 0; y < LIGHT_MAP_SIZE; y++)
		{
			float sx = (float) x / (LIGHT_MAP_SIZE - 1);
			float sy = (float) y / (LIGHT_MAP_SIZE - 1);

			Vector3 off = orientation.vx * sx + orientation.vy * sy;
			Vector3 rayPosition = orientation.origin + off;

			float light = approximation(rayPosition, orientation.normal);

			setLightMap(lightMap->idxSubLightMap, x, y, light);
		}
	}
}
