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

#include "callback.h"
#include "texture.h"
#include "lightmap.h"
#include "progress.h"

#include "functions.h"

#include "debug.h"

#include <GL/glu.h>

#include <stdio.h>
#include <math.h>

#define LINEAR_FILTER 1

#if (MOUSE_CONTROL)
# define CAMERA_MOVE_TIME_CONSTANT 100.0f
#else
# define CAMERA_MOVE_TIME_CONSTANT 5.0f
#endif

#define THIS_CGM_VERSION 1

#define BOTTOM -0.0f

Vector3 sgCamera;
Vector3 sgLookat;

int sgCntVertices;

int sgMaxPlates;
int sgMaxQuads;
int sgMaxVertices;

int gFogActive = 0;

Vector3* sgVertices;
Vector3* sgNormals;

Level sgLevel;

Vector3 sgForward;
Vector3 sgRight;

static const int gEdgeX[4] = { 0, 1, 1, 0 };
static const int gEdgeY[4] = { 0, 0, 1, 1 };

static LightMap gLightMapFloor;
static LightMap* gDataWalls[4];


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

void moveCamera(float interval, Vector3 camera, Vector3 lookat) {
	Vector3 diff;
	Vector3 up = { 0.0f, 0.0f, 1.0f };
	float error;


	/* new values */
	diff = sub(camera, sgCamera);
	error = len(diff);
	sgCamera = add(sgCamera, scale(5.0f * interval * error, norm(diff)));

	diff = sub(lookat, sgLookat);
	error = len(diff);
	sgLookat = add(sgLookat, scale(5.0f * interval * error, norm(diff)));

	/* set camera */
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	gluLookAt(
			sgCamera.x,  sgCamera.y, sgCamera.z,
			sgLookat.x, sgLookat.y, sgLookat.z,
			up.x, up.y, up.z
			);

	glGetFloatv(GL_MODELVIEW_MATRIX, &sgWindowViewport.view[0][0]);
}

int getFieldEdgeHeight(int x, int y, int edge) {
	if (x >= 0 && y >= 0 && x < sgLevel.size.x && y < sgLevel.size.y) {
		Plate* p = &sgLevel.field[x][y];

		return 5 + p->z + (gEdgeX[edge] * 2 - 1) * p->dzx + (gEdgeY[edge] * 2 - 1) * p->dzy;
	}

	/* value out of range */
	return 0;
}

void updateXSideSquareTexCoords(Square* square, int x, int z, float z1, float z2, int flip) {
	float a = (float) (LIGHT_MAP_SIZE - 1) / LIGHT_MAP_SIZE;
	float b = 1.0f / (2 * LIGHT_MAP_SIZE);

	int i;

	for (i = 0; i < 4; i++)
	{
		float t = square->vertices[i].x - x;

		if (flip) {
			t = 1.0f - t;
		}

		square->texcoords[i].x = square->vertices[i].x;
		square->texcoords[i].y = ((1.0f - t) * z1 + t * z2) - square->vertices[i].z;
		square->lightmap[i].x = (x + a * (square->vertices[i].x - x) + b) / sgLevel.size.x;
		square->lightmap[i].y = (z + a * (square->vertices[i].z - z) + b) / (MAX_LEVEL_HEIGHT + 1);
	}
}

void updateYSideSquareTexCoords(Square* square, int y, int z, float z1, float z2, int flip) {
	float a = (float) (LIGHT_MAP_SIZE - 1) / LIGHT_MAP_SIZE;
	float b = 1.0f / (2 * LIGHT_MAP_SIZE);

	int i;

	for (i = 0; i < 4; i++)
	{
		float t = square->vertices[i].y - y;

		if (flip) {
			t = 1.0f - t;
		}

		square->texcoords[i].x = square->vertices[i].y;
		square->texcoords[i].y = ((1.0f - t) * z1 + t * z2) - square->vertices[i].z;
		square->lightmap[i].x = (y + a * (square->vertices[i].y - y) + b) / sgLevel.size.y;
		square->lightmap[i].y = (z + a * (square->vertices[i].z - z) + b) / (MAX_LEVEL_HEIGHT + 1);
	}
}

void updateSquareAttributes(Square* square) {
	square->mid = midpoint(square->vertices);

	square->area = 0.5f * len(cross(sub(square->vertices[1], square->vertices[0]), sub(square->vertices[3], square->vertices[0]))) +
				         0.5f * len(cross(sub(square->vertices[1], square->vertices[2]), sub(square->vertices[3], square->vertices[2])));

}

void updatePlate(int x, int y) {
	Plate* p = &sgLevel.field[x][y];

	if (p->dirty) {
		Square* square = &p->roof;
		int i;
		int side;
		int dzx = p->dzx;
		int	dzy = p->dzy;

		float a = (float) (LIGHT_MAP_SIZE - 1) / LIGHT_MAP_SIZE;
		float b = 1.0f / (2 * LIGHT_MAP_SIZE);

		Vector3 ex;
		Vector3 ey;

		ex.x = 0.5f;
		ex.y = 0.0f;
		ex.z = (float) dzx / HEIGHT_STEPS;

		ey.x = 0.0f;
		ey.y = 0.5f;
		ey.z = (float) dzy / HEIGHT_STEPS;

		square->normal = norm(cross(ex, ey));

		for (i = 0; i < 4; i++) {
			square->vertices[i].x = x + gEdgeX[i];
			square->vertices[i].y = y + gEdgeY[i];
			square->vertices[i].z = (float) getFieldEdgeHeight(x, y, i) / HEIGHT_STEPS;
		}

		for (i = 0; i < 4; i++) {
			square->texcoords[i].x = gEdgeX[i] * (int) (len(sub(square->vertices[1], square->vertices[0])) + 0.5f);
			square->texcoords[i].y = gEdgeY[i] * (int) (len(sub(square->vertices[3], square->vertices[0])) + 0.5f);
			square->lightmap[i].x = (x + a * gEdgeX[i] + b) / sgLevel.size.x;
			square->lightmap[i].y = (y + a * gEdgeY[i] + b) / sgLevel.size.y;
			square->lightmap[i] = transformCoords(&gLightMapFloor, square->lightmap[i]);
		}

		updateSquareAttributes(square);

		for (side = 0; side < 4; side++)
		{
			int j = (side + 1) % 4;

			int i2 = (side + 3) % 4;
			int j2 = (side + 2) % 4;

			int height1 = getFieldEdgeHeight(x, y, side);
			int height2 = getFieldEdgeHeight(x, y, j);

			int dx = gEdgeY[j] - gEdgeY[side];
			int dy = gEdgeX[side] - gEdgeX[j];

			p->cntSideSquares[side] = 0;

			if (height1 > getFieldEdgeHeight(x + dx, y + dy, i2) ||
					height2 > getFieldEdgeHeight(x + dx, y + dy, j2))  {
				int x1 = x + gEdgeX[side];
				int y1 = y + gEdgeY[side];
				int x2 = x + gEdgeX[j];
				int y2 = y + gEdgeY[j];

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
				} else {
					minHeight = height1;
					maxHeight = height2;
					x3 = x2;
					y3 = y2;
				}

				for (bottom = 0; bottom < minHeight; ) {
					int top = min(minHeight, bottom + HEIGHT_STEPS);
					square = &p->sides[side][p->cntSideSquares[side]++];

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

					if (side % 2 == 0)
					{
						updateXSideSquareTexCoords(square, x, bottom / HEIGHT_STEPS, p->roof.vertices[side].z, p->roof.vertices[j].z, side / 2);
					} else {
						updateYSideSquareTexCoords(square, y, bottom / HEIGHT_STEPS, p->roof.vertices[side].z, p->roof.vertices[j].z, side / 2);
					}

					for (i = 0; i < 4; i++) {
						square->lightmap[i] = transformCoords(&gDataWalls[side][(side % 2) ? x : y], square->lightmap[i]);
					}

					updateSquareAttributes(square);

					bottom = top;
				}

				for (; bottom < maxHeight; ) {
					int top = min(maxHeight, bottom - (bottom % HEIGHT_STEPS) + HEIGHT_STEPS);

					float t1 = (float) (bottom - minHeight) / (maxHeight - minHeight);
					float t2 = (float) (top - minHeight) / (maxHeight - minHeight);

					square = &p->sides[side][p->cntSideSquares[side]++];

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

					if (side % 2 == 0) {
						updateXSideSquareTexCoords(square, x, bottom / HEIGHT_STEPS, p->roof.vertices[side].z, p->roof.vertices[j].z, side / 2);
					} else {
						updateYSideSquareTexCoords(square, y, bottom / HEIGHT_STEPS, p->roof.vertices[side].z, p->roof.vertices[j].z, side / 2);
					}

					for (i = 0; i < 4; i++) {
						square->lightmap[i] = transformCoords(&gDataWalls[side][(side % 2) ? x : y], square->lightmap[i]);
					}

					updateSquareAttributes(square);

					bottom = top;
				}
			}
		}

		p->dirty = 0;
	}
}

void getRoofSquare(int x, int y, Square* square) {
	Plate* p = &sgLevel.field[x][y];
	updatePlate(x, y);
	*square = p->roof;
}

int getSideSquares(int x, int y, int side, Square** square) {
	Plate* p = &sgLevel.field[x][y];
	updatePlate(x, y);
	*square = p->sides[side];

	return p->cntSideSquares[side];
}

GLuint genTexture(void) {
	GLuint texID;

	glGenTextures(1, &texID);
	glBindTexture(GL_TEXTURE_2D, texID);

	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
#if LINEAR_FILTER
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
#else
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
#endif

	return texID;
}

void initTextures(void) {
	int x;
	int y;

	sgLevel.lightMap = genTexture();

	allocLightMap(sgLevel.size.x * sgLevel.size.y + 4 * sgLevel.size.x * sgLevel.size.y * (MAX_LEVEL_HEIGHT + 1));

	allocSubLightMaps(&gLightMapFloor, sgLevel.size.x, sgLevel.size.y);

	MALLOC(gDataWalls[0], sgLevel.size.y * sizeof(**gDataWalls));
	MALLOC(gDataWalls[1], sgLevel.size.x * sizeof(**gDataWalls));
	MALLOC(gDataWalls[2], sgLevel.size.y * sizeof(**gDataWalls));
	MALLOC(gDataWalls[3], sgLevel.size.x * sizeof(**gDataWalls));

	for (x = 0; x < sgLevel.size.x; x++) {
		allocSubLightMaps(&gDataWalls[1][x], sgLevel.size.y, MAX_LEVEL_HEIGHT + 1);
		allocSubLightMaps(&gDataWalls[3][x], sgLevel.size.y, MAX_LEVEL_HEIGHT + 1);
	}

	for (y = 0; y < sgLevel.size.y; y++) {
		allocSubLightMaps(&gDataWalls[0][y], sgLevel.size.x, MAX_LEVEL_HEIGHT + 1);
		allocSubLightMaps(&gDataWalls[2][y], sgLevel.size.x, MAX_LEVEL_HEIGHT + 1);
	}
}

float approximationSquare(const Vector3 position, const Vector3 normal, const Square square) {
	Vector3 d;
	float r;
	float d1;
	float d2;

	d = sub(square.mid, position);

	r = len(d);

	d = scale(1.0f / r, d);

	d1 = dot(d, normal);
	d2 = dot(d, square.normal);

	if (d1 <= 0.0f || d2 <= 0.0f) {
		return 1.0f;
	}

	return 1.0f - ((d1 * d2) / (1.0f + PI * sqr(r) / square.area));
}

float approximation(const Vector3 position, const Vector3 normal) {
	Vector3 z = vector3(0.0f, 0.0f, 1.0f);
	float light = 1.0f - acos(dot(normal, z)) / PI;

	int x;
	int y;

	for (x = 0; x < sgLevel.size.x; x++) {
		for (y = 0; y < sgLevel.size.y; y++) {
			int check = 0;

			Square square;
			int j;

			if (len(sub(position, vector3(x + 0.5f, y + 0.5f, position.z))) > 6.0f) {
				continue;
			}


			getRoofSquare(x, y, &square);

			for (j = 0; j < 4 && !check; j++) {
				if (dot(sub(square.vertices[j], position), normal) > 0.0f) {
					check = 1;
				}
			}

			if (!check) {
				continue;
			}

			light *= approximationSquare(position, normal, square);

			for (j = 0; j < 4; j++) {
				Square* squares;
				int cnt = getSideSquares(x, y, j, &squares);
				int k;
				for (k = 0; k < cnt; k++) {
					light *= approximationSquare(position, normal, squares[k]);
				}
			}
		}
	}

	return light;
}

void makeAmbientOcclusionTexture(LightMap* lightMap, int fx, int fy) {
	int x;
	int y;
	Square square;

	getRoofSquare(fx, fy, &square);

	for (x = 0; x < LIGHT_MAP_SIZE; x++) {
		for (y = 0; y < LIGHT_MAP_SIZE; y++) {
			Vector3 vx = sub(square.vertices[1], square.vertices[0]);
			Vector3 vy = sub(square.vertices[3], square.vertices[0]);
			float sx = (float) x / (LIGHT_MAP_SIZE - 1);
			float sy = (float) y / (LIGHT_MAP_SIZE - 1);
			Vector3 off = add(scale(sx, vx), scale(sy, vy));
			Vector3 rayPosition = add(square.vertices[0], off);

			float light = approximation(rayPosition, square.normal);

			setLightMap(lightMap, fx * LIGHT_MAP_SIZE + x, fy * LIGHT_MAP_SIZE + y, light);
		}
	}
}

void makeAmbientOcclusionXSideTexture(LightMap* lightMap, int side, int fx, int fy) {
	Square* squares;
	int cnt = getSideSquares(fx, fy, side, &squares);
	float maxZ = 0.0f;

	int x;
	int y;
	int k;

	if (cnt == 0) {
		return;
	}

	for (k = 0; k < cnt; k++) {
		maxZ = max(maxZ, getMaxZValue(&squares[k]));
	}

	for (x = 0; x < LIGHT_MAP_SIZE; x++) {
		for (y = 0; y < (int) ceil(maxZ) * LIGHT_MAP_SIZE; y++) {
			int tx = fx * LIGHT_MAP_SIZE + x;
			int ty = y;

			float sx = fx + (float) x / (LIGHT_MAP_SIZE - 1);
			float sy = (side / 2) + (float) fy;
			float sz = (ty / LIGHT_MAP_SIZE) + (float) (ty % LIGHT_MAP_SIZE) / (LIGHT_MAP_SIZE - 1);

			Vector3 rayPosition = vector3(sx, sy, sz);

			float light = approximation(rayPosition, squares[0].normal);

			setLightMap(lightMap, tx, ty, light);
		}
	}
}

void makeAmbientOcclusionYSideTexture(LightMap* lightMap, int side, int fx, int fy) {
	Square* squares;
	int cnt = getSideSquares(fx, fy, side, &squares);
	float maxZ = 0.0f;

	int x;
	int y;
	int k;

	if (cnt == 0) {
		return;
	}

	for (k = 0; k < cnt; k++) {
		maxZ = max(maxZ, getMaxZValue(&squares[k]));
	}

	for (x = 0; x < LIGHT_MAP_SIZE; x++) {
		for (y = 0; y < (int) ceil(maxZ) * LIGHT_MAP_SIZE; y++) {
			int tx = fy * LIGHT_MAP_SIZE + x;
			int ty = y;

			float sx = 1 - (side / 2) + (float) fx;
			float sy = fy + (float) x / (LIGHT_MAP_SIZE - 1);
			float sz = (ty / LIGHT_MAP_SIZE) + (float) (ty % LIGHT_MAP_SIZE) / (LIGHT_MAP_SIZE - 1);

			Vector3 rayPosition = vector3(sx, sy, sz);

			float light = approximation(rayPosition, squares[0].normal);

			setLightMap(lightMap, tx, ty, light);
		}
	}
}

void updateTextures(int verbose) {
	int x;
	int y;

	int steps = 0;
	int cntSteps = 2 * sgLevel.size.x * sgLevel.size.y;

	if (verbose) {
		printf("calculating lightmaps...\n");
		resetProgress();
	}

	/*****/

	for (x = 0; x < sgLevel.size.x; x++) {
		for (y = 0; y < sgLevel.size.y; y++) {
			makeAmbientOcclusionTexture(&gLightMapFloor, x, y);

			if (verbose) {
				steps++;
				setProgress((float) steps / cntSteps);
			}
		}
	}

	for (x = 0; x < sgLevel.size.x; x++) {
		for (y = 0; y < sgLevel.size.y; y++) {
			makeAmbientOcclusionXSideTexture(&gDataWalls[0][y], 0, x, y);
			makeAmbientOcclusionYSideTexture(&gDataWalls[1][x], 1, x, y);
			makeAmbientOcclusionXSideTexture(&gDataWalls[2][y], 2, x, y);
			makeAmbientOcclusionYSideTexture(&gDataWalls[3][x], 3, x, y);

			if (verbose) {
				steps++;
				setProgress((float) steps / cntSteps);
			}
		}
	}

	/*****/

	lightMapToTexture(sgLevel.lightMap);
}

void initCommon(void) {
	resetCamera();
}

void destroyCommon(void) {
	FREE(gDataWalls[0]);
	FREE(gDataWalls[1]);
	FREE(gDataWalls[2]);
	FREE(gDataWalls[3]);

	freeLightMap();

	FREE(sgLevel.field[0]);
	FREE(sgLevel.field);
	FREE(sgVertices);
	FREE(sgNormals);

	sgLevel.size.x = -1;
	sgLevel.size.y = -1;
}

void resetCamera(void) {
	sgCamera.x = 0.0f;
	sgCamera.y = 0.0f;
	sgCamera.z = 1.0f;

	sgLookat.x = 0.0f;
	sgLookat.y = 0.0f;
	sgLookat.z = 0.0f;
}

void allocLevelDataMemory(void) {
	int x;

	MALLOC(sgLevel.field, sgLevel.size.x * sizeof(Plate*));
	MALLOC(sgLevel.field[0], sgLevel.size.x * sgLevel.size.y * sizeof(Plate));
	for (x = 1; x < sgLevel.size.x; x++) {
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

void newLevel(void) {
	int x, y;

	fprintf(stderr, "creating new level: (%d, %d)\n", sgLevel.size.x, sgLevel.size.y);

	sgLevel.start.x = 0;
	sgLevel.start.y = 0;

	sgLevel.finish.x = sgLevel.size.x - 1;
	sgLevel.finish.y = sgLevel.size.y - 1;

	allocLevelDataMemory();

	for (x = 0; x < sgLevel.size.x; x++) {
		for (y = 0; y < sgLevel.size.y; y++) {
			Plate* p = &sgLevel.field[x][y];
			p->z = 0;
			p->dzx = 0;
			p->dzy = 0;
		}
	}
}

int loadFieldFromFile(const char* filename) {
	FILE* file = fopen(filename, "rt");
	int x, y;
	int fileX, fileY;
	int version;

	if (sgLevel.plateTexture == 0) {
		sgLevel.plateTexture = loadTexture("data/plate.tga", 1);
	}

	if (!file) {
		fprintf(stderr, "can not open file: %s\n", filename);
		return 0;
	}

	/* version number */
	fscanf(file, "v%d", &version);

	if (version > THIS_CGM_VERSION) {
		fprintf(stderr, "incompatible version number: %d\n", version);
		return 0;
	}

	/* read attributes */
	fscanf(file, "%d %d", &sgLevel.start.x, &sgLevel.start.y);
	fscanf(file, "%d %d", &sgLevel.finish.x, &sgLevel.finish.y);
  fscanf(file, "%d %d", &fileX, &fileY);

	/* read size from file, if not given through program parameters */
	if (sgLevel.size.x < 0 || sgLevel.size.y < 0) {
		sgLevel.size.x = fileX;
		sgLevel.size.y = fileY;
	}

	allocLevelDataMemory();

	/* reading data */
	for (x = 0; x < sgLevel.size.x; x++) {
		for (y = 0; y < sgLevel.size.y; y++) {
			Plate* p = &sgLevel.field[x][y];
			if (x < fileX && y < fileY) {
				fscanf(file, "%d %d %d", &p->z, &p->dzx, &p->dzy);
			} else { /* growing */
				p->z = 0;
				p->dzx = 0;
				p->dzy = 0;
			}

			p->dirty = 1;
		}

		/* shrinking */
		if (fileY > sgLevel.size.y) {
			int dummyZ;
			int dummyDZX;
			int dummyDZY;

			for (y = sgLevel.size.y; y < fileY; y++) {
				fscanf(file, "%d %d %d", &dummyZ, &dummyDZX, &dummyDZY);
			}
		}
	}

	fclose(file);

	return 1;
}

int saveFieldToFile(const char* filename) {
	FILE* file = fopen(filename, "wt");
	int x, y;

	if (!file) return 0;

	/* version number */
	fprintf(file, "v%d\n", THIS_CGM_VERSION);

	/* write atributes */
	fprintf(file, "%d %d\n", sgLevel.start.x, sgLevel.start.y);
	fprintf(file, "%d %d\n", sgLevel.finish.x, sgLevel.finish.y);
	fprintf(file, "%d %d\n", sgLevel.size.x, sgLevel.size.y);

	/* write data */
	for (x = 0; x < sgLevel.size.x; x++) {
		for (y = 0; y < sgLevel.size.y; y++) {
			Plate* p = &sgLevel.field[x][y];
			fprintf(file, "%d %d %d\n", p->z, p->dzx, p->dzy);
		}
	}

	if (fclose(file) != 0) {
		return 0;
	}

	return 1;
}
