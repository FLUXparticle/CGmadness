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

#include "math/Vector3.hpp"

#include "Color.hpp"

#include <cstdio>
#include <cstdlib>
#include <cmath>

#define MAP_SIZE_X 50
#define MAP_SIZE_Y 50

#define NOISE_GRID_SIZE 64

static Vector3 gGrid[NOISE_GRID_SIZE][NOISE_GRID_SIZE];

float randf(void)
{
	return (float) rand() / RAND_MAX;
}

Vector3 randv(void)
{
	Vector3 a(randf() * 2.0f - 1.0f, randf() * 2.0f - 1.0f, randf() * 2.0f - 1.0f);
	return a.norm();
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

	for (int i = 0; i < 8; i++)
	{
		int iix = ix + i % 2;
		int iiy = iy + (i / 2) % 2;
		int iiz = iz + i / 4;
		Vector3 v = p - Vector3(iix, iiy, iiz);

		k[i] = v * gGrid[(iix + iiz) % NOISE_GRID_SIZE][(iiy + iiz) % NOISE_GRID_SIZE];
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

	for (i = 0; i < 4; i++)
	{
		value += a * noise(f * v);

		a *= 0.75;
		f *= 2.0f;
	}

	return value;
}

struct Map
{
	int data[MAP_SIZE_X][MAP_SIZE_Y];
};

void genNoiseTexture(Map& map, Vector3 origin, Vector3 vx, Vector3 vy)
{
	int x;
	int y;

	for (x = 0; x < MAP_SIZE_X; x++)
	{
		for (y = 0; y < MAP_SIZE_Y; y++)
		{
			float sx = (float) x / MAP_SIZE_X;
			float sy = (float) y / MAP_SIZE_Y;

			Vector3 off = sx * vx + sy * vy;
			Vector3 v = origin + off;

			float n = (perlinNoise(v) + 1.0f) / 2.0f;
			map.data[x][y] = (int) (n * 8);
		}
	}
}

int main(int argc, char* argv[])
{
	Map map;
	Vector3 origin(0.0f, 0.0f, 0.0f);
	Vector3 vx(10.0f, 0.0f, 0.0f);
	Vector3 vy(0.0f, 10.0f, 0.0f);

	initNoise();

	genNoiseTexture(map, origin, vx, vy);

	for (int y = 0; y < MAP_SIZE_Y; y++)
	{
		for (int x = 0; x < MAP_SIZE_X; x++)
		{
			int water = 4;
			int height = map.data[x][y];
			if (height > water)
			{
				printf("%3d", height - water);
			}
			else
			{
				printf("   ");
			}
		}
		printf("\n");
	}

	return 0;
}
