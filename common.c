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

#include "debug.h"

#include <GL/glu.h>

#include <stdio.h>
#include <math.h>

#define ANGLESTEP 1.0f
#define ZOOMSTEP 0.5f

#if (MOUSE_CONTROL)
#  define CAMERA_MOVE_TIME_CONSTANT 100.0f
#else
#  define CAMERA_MOVE_TIME_CONSTANT 5.0f
#endif

#define THIS_CGM_VERSION 1

Vector3 sgCamera = { 0.0f, 0.0f, 1.0f };
Vector3 sgLookat = { 0.0f, 0.0f, 0.0f };

int sgCntVertices;

int sgMaxPlates;
int sgMaxQuads;
int sgMaxVertices;

int gFogActive = 0;

Vector3* sgVertices;
Vector3* sgNormals;
int* sgIndexVertices;
int* sgQuadInShadow;

Level sgLevel;

Vector3 sgForward;
Vector3 sgRight;

static int gEdgeX[4] = { 0, 1, 1, 0 };
static int gEdgeY[4] = { 0, 0, 1, 1 };

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

		return p->z + (gEdgeX[edge] * 2 - 1) * p->dzx + (gEdgeY[edge] * 2 - 1) * p->dzy;
	}

	/* value out of range */
	return -5;
}

void getRoofSquare(int x, int y, Square* square) {
	int i;
	Plate* p = &sgLevel.field[x][y];
	int dzx = p->dzx;
	int	dzy = p->dzy;
	
	Vector3 ex;
	Vector3 ey;

	ex.x = 0.5f;
	ex.y = 0.0f;
	ex.z = SCALE_Z * dzx;
	
	ey.x = 0.0f;
	ey.y = 0.5f;
	ey.z = SCALE_Z * dzy;
	
	square->normal = norm(cross(ex, ey));

	for (i = 0; i < 4; i++) {
		square->vertices[i].x = x + gEdgeX[i];
		square->vertices[i].y = y + gEdgeY[i];
		square->vertices[i].z = SCALE_Z * getFieldEdgeHeight(x, y, i);
	}

	for (i = 0; i < 4; i++) {
		square->texcoords[i].x = gEdgeX[i];
		square->texcoords[i].y = gEdgeY[i];
	}
}

int getSideSquare(int x, int y, int side, Square* square) {
	Square roofsquare;
	int j = (side + 1) % 4;
	int dx;
	int dy;

	int i2 = (side + 3) % 4;
	int j2 = (side + 2) % 4;

	getRoofSquare(x, y, &roofsquare);

	dx = roofsquare.vertices[j].y - roofsquare.vertices[side].y;
	dy = -(roofsquare.vertices[j].x - roofsquare.vertices[side].x);

	if (getFieldEdgeHeight(x, y, side) <= getFieldEdgeHeight(x + dx, y + dy, i2) && getFieldEdgeHeight(x, y, j) <= getFieldEdgeHeight(x + dx, y + dy, j2))  {
		return 0;
	}

	square->normal.x = dx;
	square->normal.y = dy;
	square->normal.z = 0.0f;
	
	 
	square->texcoords[0].x = 0.0f;
	square->texcoords[0].y = 0.0f;
	
	square->vertices[0] = roofsquare.vertices[side];


	square->texcoords[1].x = 0.0f;
	square->texcoords[1].y = roofsquare.vertices[side].z + 0.5f;	
	
	square->vertices[1].x = roofsquare.vertices[side].x;
	square->vertices[1].y = roofsquare.vertices[side].y;
	square->vertices[1].z = -0.5f;
	

	square->texcoords[2].x = 1.0f;
	square->texcoords[2].y = roofsquare.vertices[j].z + 0.5f;

	square->vertices[2].x = roofsquare.vertices[j].x;
	square->vertices[2].y = roofsquare.vertices[j].y;
	square->vertices[2].z = -0.5f;

	square->texcoords[3].x = 1.0f;
	square->texcoords[3].y = 0.0f;
	
	square->vertices[3] = roofsquare.vertices[j];

	return 1;
}

void getVertIndex(int x, int y, int* start, int* end) {
	if (x >= 0 && x < sgLevel.size.x && y >= 0 && y < sgLevel.size.y) {
		int index = y * sgLevel.size.x + x;
		*start = sgIndexVertices[index];
		index++;
		if (index < sgMaxPlates) {
			*end = sgIndexVertices[index];
		} else {
			*end = sgCntVertices;
		}
	} else {
		*start = 0;
		*end = 0;
	}
}

void destroyCommon(void) {
	FREE(sgLevel.field[0]);
	FREE(sgLevel.field);
	FREE(sgVertices);
	FREE(sgNormals);
	FREE(sgIndexVertices);
  FREE(sgQuadInShadow);

	sgLevel.size.x = -1;
	sgLevel.size.y = -1;
}

int loadFieldFromFile(char* filename) {
	FILE* file = fopen(filename, "rt");
	int x,y;
	int fileX, fileY;
	int version;

	if (!file) {
		fprintf(stderr, "can not open file: %s\n", filename);
		return 0;
	}

	sgLevel.texture = loadTexture("data/plate.tga", 1);

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

	/* alloc memory for data */
	MALLOC(sgLevel.field, sgLevel.size.x * sizeof(Plate*));
	MALLOC(sgLevel.field[0], sgLevel.size.x * sgLevel.size.y * sizeof(Plate));
	for (x = 1; x < sgLevel.size.x; x++) {
		sgLevel.field[x] = sgLevel.field[0] + x * sgLevel.size.y;
	}

	/* reading data */
	for (x = 0; x < sgLevel.size.x; x++) {
		for (y = 0; y < sgLevel.size.y; y++) {
			Plate* p = &sgLevel.field[x][y];
			if (x < fileX && y < fileY) {
				fscanf(file, "%d %d %d", &p->z, &p->dzx, &p->dzy);
			} else {
				p->z = 0;
				p->dzx = 0;
				p->dzy = 0;
			}
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
	
  /* init level stuff */
	sgMaxPlates = sgLevel.size.x * sgLevel.size.y;
	sgMaxQuads = 5 * sgMaxPlates;
	sgMaxVertices = 4 * sgMaxQuads;
	
	sgCntVertices = 0;
	MALLOC(sgVertices, sgMaxVertices * sizeof(Vector3));
	MALLOC(sgNormals, sgMaxVertices * sizeof(Vector3));
	MALLOC(sgIndexVertices, sgMaxPlates * sizeof(int));
	MALLOC(sgQuadInShadow, sgMaxQuads * sizeof(int));
	
	return 1;
}

int saveFieldToFile(char* filename) {
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

	fclose(file);

	return 1;
}
