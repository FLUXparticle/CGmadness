#include "NoiseWorld.hpp"

#include <cstdlib>

#define WATER 4

#define NOISE_GRID_SIZE 64

static bool initalized = false;

static Vector3 gGrid[NOISE_GRID_SIZE][NOISE_GRID_SIZE];

static float randf(void)
{
	return (float) rand() / RAND_MAX;
}

static Vector3 randv(void)
{
	Vector3 a(randf() * 2.0f - 1.0f, randf() * 2.0f - 1.0f, randf() * 2.0f - 1.0f);
	return a.norm();
}

static void initNoise(void)
{
	for (int y = 0; y < NOISE_GRID_SIZE; y++)
	{
		for (int x = 0; x < NOISE_GRID_SIZE; x++)
		{
			gGrid[y][x] = randv();
		}
	}
}

static float interpolate(float x1, float x2, float t)
{
	return (1.0f - t) * x1 + t * x2;
}

static float noise(Vector3 p)
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

static float perlinNoise(Vector3 v)
{
	float f = 1.0f;
	float a = 1.0f;

	float value = 0.0f;

	for (int i = 0; i < 4; i++)
	{
		value += a * noise(f * v);

		a *= 0.75;
		f *= 2.0f;
	}

	return value;
}


void NoiseWorld::genNoiseTexture(NoiseWorld::Map& map, Vector3 origin, Vector3 vx, Vector3 vy)
{
	for (int x = 0; x < WORLD_SIZE_X; x++)
	{
		for (int y = 0; y < WORLD_SIZE_Y; y++)
		{
			float sx = (float) x / WORLD_SIZE_X;
			float sy = (float) y / WORLD_SIZE_Y;

			Vector3 off = sx * vx + sy * vy;
			Vector3 v = origin + off;

			float n = (perlinNoise(v) + 1.0f) / 2.0f;
			map.data[x][y] = (int) (n * 8);
		}
	}
}

NoiseWorld::NoiseWorld()
{
	if (!initalized)
	{
		initNoise();
	}

	Vector3 origin(0.0f, 0.0f, 0.0f);
	Vector3 vx(10.0f, 0.0f, 0.0f);
	Vector3 vy(0.0f, 10.0f, 0.0f);

	genNoiseTexture(map, origin, vx, vy);
}

NoiseWorld::~NoiseWorld()
{
	// empty
}

int NoiseWorld::getHeight(int x, int y) const
{
	int height = map.data[x][y];
	if (height > WATER)
	{
		return height - WATER;
	}
	else
	{
		return 0;
	}
}
