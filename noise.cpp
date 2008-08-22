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

#include "noise.hpp"

#include "texture.hpp"

#include "vector.hpp"

#include <stdlib.h>

#define NOISE_GRID_SIZE 64

static Color3 gCol1;
static Color3 gCol2;

static Vector3 gGrid[NOISE_GRID_SIZE][NOISE_GRID_SIZE];

float randf(void)
{
	return (float) rand() / RAND_MAX;
}

Vector3 randv(void)
{
	Vector3 a =
		vector3(randf() * 2.0f - 1.0f, randf() * 2.0f - 1.0f,
						randf() * 2.0f - 1.0f);

	return norm(a);
}

void initNoise(void)
{
	int x;
	int y;

	for (y = 0; y < NOISE_GRID_SIZE; y++)
	{
		for (x = 0; x < NOISE_GRID_SIZE; x++)
		{
			gGrid[y][x] = randv();
		}
	}

	gCol1 = color3i(169, 102, 50);
	gCol2 = color3i(254, 180, 103);
}

static float interpolate(float x1, float x2, float t)
{
	return (1.0f - t) * x1 + t * x2;
}

float noise(Vector3 p)
{
	int ix = (int) p.x;
	int iy = (int) p.y;
	int iz = (int) p.z;

	float k[8];
	int i;

	for (i = 0; i < 8; i++)
	{
		int iix = ix + i % 2;
		int iiy = iy + (i / 2) % 2;
		int iiz = iz + i / 4;
		Vector3 v = sub(p, vector3(iix, iiy, iiz));

		k[i] =
			dot(v,
					gGrid[(iix + iiz) % NOISE_GRID_SIZE][(iiy + iiz) % NOISE_GRID_SIZE]);
	}

	{
		float fx = p.x - ix;

		float x0 = interpolate(k[0], k[1], fx);
		float x1 = interpolate(k[2], k[3], fx);

		float fy = p.y - iy;

		float y0 = interpolate(x0, x1, fy);

		float x2 = interpolate(k[4], k[5], fx);
		float x3 = interpolate(k[6], k[7], fx);

		float y1 = interpolate(x2, x3, fy);

		float fz = p.z - iz;

		return interpolate(y0, y1, fz);
	}
}

float perlinNoise(Vector3 v)
{
	float f = 1.0f;
	float a = 1.0f;

	float value = 0.0f;
	int i;

	for (i = 0; i < 8; i++)
	{
		value += a * noise(scale(f, v));

		a *= 0.75;
		f *= 2.0f;
	}

	return value;
}

void genNoiseTexture(SubAtlas * subAtlas, Vector3 origin, Vector3 vx,
										 Vector3 vy)
{
	int x;
	int y;

	for (x = 0; x < subAtlas->sizeX * COLOR_MAP_SIZE; x++)
	{
		for (y = 0; y < subAtlas->sizeY * COLOR_MAP_SIZE; y++)
		{
			float sx =
				(x / COLOR_MAP_SIZE) + (float) (x % COLOR_MAP_SIZE) / (COLOR_MAP_SIZE -
																															 1);
			float sy =
				(y / COLOR_MAP_SIZE) + (float) (y % COLOR_MAP_SIZE) / (COLOR_MAP_SIZE -
																															 1);

			Vector3 off = add(scale(sx, vx), scale(sy, vy));
			Vector3 v = add(origin, off);

			float n = (perlinNoise(v) + 1.0f) / 2.0f;
			Color3 col = interpolateColor(gCol1, gCol2, n);

			setColorMap(subAtlas, x, y, col);
		}
	}
}
