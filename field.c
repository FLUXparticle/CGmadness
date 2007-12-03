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

#include "field.h"

#include "common.h"
#include "camera.h"
#include "game.h"

#include "ball.h"
#include "features.h"

#include "vector.h"
#include "functions.h"

#include "types.h"
#include "debug.h"

#include <GL/glew.h>
#include <GL/gl.h>

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define WITHOUT_DEPTH_TEST 0
#define WITH_STENCIL_TEST 0
#define TWO_PASS 0

static Vector2 gDefaultTexCoord;
static Vector2 gDefaultColorMapCoord;
static Vector2 gDefaultLightMapCoord;
static Vector3 gDefaultNormal;
static Color4 gDefaultColor;

static Vector2* gTexCoords;
static Vector2* gColorMapCoords;
static Vector2* gLightMapCoords;
static Color4* gColors;
static unsigned int gVBuffers[6];

static Vector3* gBallShadowCoords;

static GLuint gWhiteTexture; 
static GLuint gBallShadow; 

static int gCntIndices = 0;
static int* gIndices;

static int gCntBallReflectionIndices = 0;
static int* gBallReflectionIndices;

static int gCntSpotlightIndices;
static int* gSpotlightIndices;

static int* gIndexVertices;

void setColor(Color4 col) {
	gDefaultColor = col;
}

void setTexCoord(Vector2 uv) {
	gDefaultTexCoord = uv;
}

void setColorMapCoord(Vector2 uv) {
	gDefaultColorMapCoord = uv;
}

void setLightMapCoord(Vector2 uv) {
	gDefaultLightMapCoord = uv;
}

void setNormal(Vector3 n) {
	gDefaultNormal = n;
}

void addVertex(Vector3 v) {
	gTexCoords[sgCntVertices] = gDefaultTexCoord;
	gColorMapCoords[sgCntVertices] = gDefaultColorMapCoord;
	gLightMapCoords[sgCntVertices] = gDefaultLightMapCoord;
	sgNormals[sgCntVertices] = gDefaultNormal;
	gColors[sgCntVertices] = gDefaultColor;

	sgVertices[sgCntVertices] = v;

	sgCntVertices++;
}

void addSquare(const Square* square) {
	int i;
	setNormal(square->normal);

	for (i = 0; i < 4; i++) {
		setTexCoord(square->texcoord[i]);
		setColorMapCoord(square->colormap[i]);
		setLightMapCoord(square->lightmap[i]);
		addVertex(square->vertices[i]);
	}
}

void getVertIndex(int x, int y, int* start, int* end) {
	if (x >= 0 && x < sgLevel.size.x && y >= 0 && y < sgLevel.size.y) {
		int index = y * sgLevel.size.x + x;
		*start = gIndexVertices[index];
		index++;
		if (index < sgMaxPlates) {
			*end = gIndexVertices[index];
		} else {
			*end = sgCntVertices;
		}
	} else {
		*start = 0;
		*end = 0;
	}
}

void setSquareColor(int q, Color4 col)
{
	int i;

	for (i = 0; i < 4; i++) {
		gColors[q + i] = col;
	}
}

void setRoofColor(int x, int y, Color4 col)
{
	int start;
	int end;

	getVertIndex(x, y, &start, &end);

	setSquareColor(start, col);
}

#define bufferdata(x) glBufferDataARB(GL_ARRAY_BUFFER_ARB, sizeof(*(x)) * sgCntVertices, (x), GL_STATIC_DRAW_ARB);

#define MAX_XY 1.0f
#define MAX_Z 1.0f

#define SAMPLES_XY 64
#define SAMPLES_Z 64

void initBallShadow(void)
{
	GLfloat ballShadowData[SAMPLES_Z][SAMPLES_XY][SAMPLES_XY];
	
	int x;
	int y;
	int z;
	
	float fx;
	float fy;
	float fz;
	
	float d;
	
	for (y = 0; y < SAMPLES_XY; y++)
	{
		for (x = 0; x < SAMPLES_XY; x++)
		{
			ballShadowData[0][y][x] = 1.0f;
		}
	}

	for (z = 1; z < SAMPLES_Z; z++)
	{
		fz = ((float) z / SAMPLES_Z) * MAX_Z;

		for (y = 0; y < SAMPLES_XY; y++)
		{
			fy = (((float) y / SAMPLES_XY) - 0.5f) * MAX_XY;

			for (x = 0; x < SAMPLES_XY; x++)
			{
				fx = (((float) x / SAMPLES_XY) - 0.5f) * MAX_XY;
				
				d = sqrt(sqr(fx) + sqr(fy) + sqr(fz));
				
				ballShadowData[z][y][x] = 1.0f - (fz / d) / (1.0f + sqr(d) / sqr(sgoBall.radius));
			}
		}
	}
	
	glGenTextures(1, &gBallShadow);
	glBindTexture(GL_TEXTURE_3D, gBallShadow);
	
	glTexImage3D(GL_TEXTURE_3D, 0, GL_LUMINANCE, SAMPLES_XY, SAMPLES_XY, SAMPLES_Z, 0, GL_LUMINANCE, GL_FLOAT, &ballShadowData[0][0][0]);

	glTexParameterf(GL_TEXTURE_3D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameterf(GL_TEXTURE_3D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameterf(GL_TEXTURE_3D, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

	glTexParameterf(GL_TEXTURE_3D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_3D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
}

void initGameField(void)
{
	static Color4 green = { 0.0f, 1.0f, 0.0f, 1.0f };
	static Color4 blue  = { 0.0f, 0.0f, 1.0f, 1.0f };
	static Color4 white = { 1.0f, 1.0f, 1.0f, 1.0f };

	int x;
	int y;
	int i;
	int index = 0;
	
	glGenTextures(1, &gWhiteTexture);
	
	glBindTexture(GL_TEXTURE_2D, gWhiteTexture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_LUMINANCE, 1, 1, 0, GL_LUMINANCE, GL_FLOAT, &white.r);
	
	initBallShadow();

	MALLOC(gIndexVertices, sgMaxPlates * sizeof(int));
  MALLOC(gTexCoords, sgMaxVertices * sizeof(Vector2));
  MALLOC(gColorMapCoords, sgMaxVertices * sizeof(Vector2));
  MALLOC(gLightMapCoords, sgMaxVertices * sizeof(Vector2));
	MALLOC(gColors, sgMaxVertices * sizeof(Color4));
	
  MALLOC(gBallShadowCoords, sgMaxVertices * sizeof(Vector3));

	setColor(white);

	for (y = 0; y < sgLevel.size.y; y++) {
		for (x = 0; x < sgLevel.size.x; x++) {
			Square square;

			gIndexVertices[index++] = sgCntVertices;

			getRoofSquare(x, y, &square);

			addSquare(&square);

			for (i = 0; i < 4; i++) {
				SideFace face;
				int k;

				getSideFace(x, y, i, &face);

				for (k = 0; k < face.cntSquares; k++) {
					addSquare(&face.squares[k]);
				}
			}
		}
	}

	setRoofColor(sgLevel.start.x, sgLevel.start.y, green);
	setRoofColor(sgLevel.finish.x, sgLevel.finish.y, blue);

	if (hasVertexbuffer()) {
		glGenBuffersARB(LENGTH(gVBuffers), gVBuffers);

		glBindBufferARB(GL_ARRAY_BUFFER_ARB, gVBuffers[0]);
		bufferdata(sgVertices);

		glBindBufferARB(GL_ARRAY_BUFFER_ARB, gVBuffers[1]);
		bufferdata(sgNormals);

		glBindBufferARB(GL_ARRAY_BUFFER_ARB, gVBuffers[2]);
		bufferdata(gColorMapCoords);

		glBindBufferARB(GL_ARRAY_BUFFER_ARB, gVBuffers[3]);
		bufferdata(gLightMapCoords);

		glBindBufferARB(GL_ARRAY_BUFFER_ARB, gVBuffers[4]);
		bufferdata(gTexCoords);

		glBindBufferARB(GL_ARRAY_BUFFER_ARB, gVBuffers[5]);
		bufferdata(gColors);

		glBindBufferARB(GL_ARRAY_BUFFER_ARB, 0);
	}

	printf("sgMaxVertices: %d\n", sgMaxVertices);
	printf("sgCntVertices: %d\n", sgCntVertices);

	gCntIndices = 0;
	MALLOC(gIndices, sgCntVertices * sizeof(int));
	MALLOC(gBallReflectionIndices, sgCntVertices * sizeof(int));

	if (hasSpotlight())
	{
		gCntSpotlightIndices = 0;
		MALLOC(gSpotlightIndices, sgCntVertices * sizeof(int));
	}
}

void destroyGameField(void)
{
	if (hasSpotlight())
	{
  	FREE(gSpotlightIndices);
	}

	FREE(gIndexVertices);
  FREE(gIndices);
  FREE(gBallReflectionIndices);
	FREE(gTexCoords);
	FREE(gColorMapCoords);
	FREE(gLightMapCoords);
	FREE(gColors);

  FREE(gBallShadowCoords);
}

/*
 * render from close to far
 */
void bsp(int startX, int startY, int sizeX, int sizeY, int viewX, int viewY, int farToClose, int* indices, int* index)
{
	if (sizeX == 0 || sizeY == 0)
	{
		return;
	}
	else if (sizeX == 1 && sizeY == 1)
	{
		int start;
		int end;
		int q;

		getVertIndex(startX, startY, &start, &end);

		for (q = start; q < end; q++)
		{
			indices[(*index)++] = q;
		}
	}
	else
	{
		int startX1 = startX;
		int startY1 = startY;
		int sizeX1 = sizeX;
		int sizeY1 = sizeY;

		int startX2 = startX;
		int startY2 = startY;
		int sizeX2 = sizeX;
		int sizeY2 = sizeY;

		int closer;

		if (sizeX > sizeY)
		{
			sizeX1 = sizeX / 2;
			sizeX2 = sizeX - sizeX1;
			startX2 = startX1 + sizeX1;
			closer = viewX < startX2;
		}
		else
		{
			sizeY1 = sizeY / 2;
			sizeY2 = sizeY - sizeY1;
			startY2 = startY1 + sizeY1;
			closer = viewY < startY2;
		}

		if (farToClose ^ closer)
		{
			bsp(startX1, startY1, sizeX1, sizeY1, viewX, viewY, farToClose, indices, index);
			bsp(startX2, startY2, sizeX2, sizeY2, viewX, viewY, farToClose, indices, index);
		}
		else
		{
			bsp(startX2, startY2, sizeX2, sizeY2, viewX, viewY, farToClose, indices, index);
			bsp(startX1, startY1, sizeX1, sizeY1, viewX, viewY, farToClose, indices, index);
		}
	}
}

void updateGameField(void)
{
	static int lastMX = 0;
	static int lastMY = 0;

	int mx = floor(sgCamera.x);
	int my = floor(sgCamera.y);
	
	int q;

	if (gCntIndices == 0 || !(mx == lastMX && my == lastMY))
	{
		gCntIndices = 0;
		bsp(0, 0, sgLevel.size.x, sgLevel.size.y, mx, my, 0, gIndices, &gCntIndices);

		lastMX = mx;
		lastMY = my;
	}

	for (q = 0; q < sgCntVertices; q += 4)
	{
		Vector3 vz = sgNormals[q];
		Vector3 vx = norm(sub(sgVertices[q + 1], sgVertices[q])); 
		Vector3 vy = norm(cross(vx, vz)); 
		
		int i;
		
		for (i = 0; i < 4; i++)
		{
			Vector3 vertex = sgVertices[q + i];
			Vector3 d = sub(sgoBall.pos, vertex);

			float x = dot(vx, d) / MAX_XY + 0.5f;
			float y = dot(vy, d) / MAX_XY + 0.5f;
			float z = dot(vz, d) / MAX_Z;

		  gBallShadowCoords[q + i] = vector3(x, y, z);
		}
	}

	if (useBallReflection())
	{
		static int lastBX = 0;
		static int lastBY = 0;

		int bx = floor(sgoBall.pos.x);
		int by = floor(sgoBall.pos.y);

		if (gCntBallReflectionIndices == 0 || !(bx == lastBX && by == lastBY))
		{
			gCntBallReflectionIndices = 0;
			bsp(0, 0, sgLevel.size.x, sgLevel.size.y, bx, by, 1, gBallReflectionIndices, &gCntBallReflectionIndices);

			lastBX = bx;
			lastBY = by;
		}
	}
}

void drawGameField(int ballReflection)
{
	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_NORMAL_ARRAY);

  glClientActiveTextureARB(GL_TEXTURE0);
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);

	if (useShadows())
	{
		glClientActiveTextureARB(GL_TEXTURE1);
		glEnableClientState(GL_TEXTURE_COORD_ARRAY);
	}

  glClientActiveTextureARB(GL_TEXTURE2);
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);

	glEnableClientState(GL_COLOR_ARRAY);

	if (hasVertexbuffer())
	{
		glBindBufferARB(GL_ARRAY_BUFFER_ARB, gVBuffers[0]);
		glVertexPointer(3, GL_FLOAT, 0, NULL);

		glBindBufferARB(GL_ARRAY_BUFFER_ARB, gVBuffers[1]);
		glNormalPointer(GL_FLOAT, 0, NULL);

	  glClientActiveTextureARB(GL_TEXTURE0);
		glBindBufferARB(GL_ARRAY_BUFFER_ARB, gVBuffers[2]);
		glTexCoordPointer(2, GL_FLOAT, 0, NULL);

		if (useShadows())
		{
			glClientActiveTextureARB(GL_TEXTURE1);
			glBindBufferARB(GL_ARRAY_BUFFER_ARB, gVBuffers[3]);
			glTexCoordPointer(2, GL_FLOAT, 0, NULL);
		}

	  glClientActiveTextureARB(GL_TEXTURE2);
		glBindBufferARB(GL_ARRAY_BUFFER_ARB, gVBuffers[4]);
		glTexCoordPointer(2, GL_FLOAT, 0, NULL);

		glBindBufferARB(GL_ARRAY_BUFFER_ARB, gVBuffers[5]);
		glColorPointer(4, GL_FLOAT, 0, NULL);
	} else {
		glVertexPointer(3, GL_FLOAT, 0, sgVertices);

		glNormalPointer(GL_FLOAT, 0, sgNormals);

	  glClientActiveTextureARB(GL_TEXTURE0);
		glTexCoordPointer(2, GL_FLOAT, 0, gColorMapCoords);

		if (useShadows())
		{
			glClientActiveTextureARB(GL_TEXTURE1);
			glTexCoordPointer(2, GL_FLOAT, 0, gLightMapCoords);
		}

	  glClientActiveTextureARB(GL_TEXTURE2);
		glTexCoordPointer(2, GL_FLOAT, 0, gTexCoords);

		glColorPointer(4, GL_FLOAT, 0, gColors);
	}

  glActiveTexture(GL_TEXTURE0);
  glEnable(GL_TEXTURE_2D);
#if (NOISE_TEXTURE)
	glBindTexture(GL_TEXTURE_2D, sgLevel.colorMap);
#else
	glBindTexture(GL_TEXTURE_2D, gWhiteTexture);
#endif

  glActiveTexture(GL_TEXTURE1);
	glEnable(GL_TEXTURE_2D);
	if (useShadows())
	{
		glBindTexture(GL_TEXTURE_2D, sgLevel.lightMap);
	}
	else
	{
		glBindTexture(GL_TEXTURE_2D, gWhiteTexture);
	}

  glActiveTexture(GL_TEXTURE2);
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, sgLevel.borderTexture);

	if (useSpotlight())
	{
		glUseProgram(sgSpotlightShader);

		glUniform3fv(glGetUniformLocation(sgSpotlightShader, "ball"), 1, &sgoBall.pos.x);
		glUniform1i(glGetUniformLocation(sgSpotlightShader, "tex0"), 0);
		glUniform1i(glGetUniformLocation(sgSpotlightShader, "tex1"), 1);
		glUniform1i(glGetUniformLocation(sgSpotlightShader, "tex2"), 2);
	}
	else
	{
	  glActiveTexture(GL_TEXTURE3);
		glEnable(GL_TEXTURE_3D);
		glBindTexture(GL_TEXTURE_3D, gBallShadow);

	  glClientActiveTextureARB(GL_TEXTURE3);
		glEnableClientState(GL_TEXTURE_COORD_ARRAY);
		glTexCoordPointer(3, GL_FLOAT, 0, gBallShadowCoords);
	}

		glDrawElements(GL_QUADS, gCntIndices, GL_UNSIGNED_INT, ballReflection ? gBallReflectionIndices : gIndices);

	if (useSpotlight())
	{
		glUseProgram(0);
	}
	else
	{
	  glActiveTexture(GL_TEXTURE3);
		glDisable(GL_TEXTURE_3D);

		glClientActiveTextureARB(GL_TEXTURE3);
		glDisableClientState(GL_TEXTURE_COORD_ARRAY);
	}

  glActiveTexture(GL_TEXTURE2);
	glDisable(GL_TEXTURE_2D);

	if (useShadows())
	{
		glActiveTexture(GL_TEXTURE1);
		glDisable(GL_TEXTURE_2D);
	}

  glActiveTexture(GL_TEXTURE0);
	glDisable(GL_TEXTURE_2D);

	if (hasVertexbuffer()) {
		glBindBufferARB(GL_ARRAY_BUFFER_ARB, 0);
	}

	glDisableClientState(GL_VERTEX_ARRAY);
	glDisableClientState(GL_NORMAL_ARRAY);

  glClientActiveTextureARB(GL_TEXTURE2);
	glDisableClientState(GL_TEXTURE_COORD_ARRAY);

	if (useShadows())
	{
		glClientActiveTextureARB(GL_TEXTURE1);
		glDisableClientState(GL_TEXTURE_COORD_ARRAY);
	}

  glClientActiveTextureARB(GL_TEXTURE0);
	glDisableClientState(GL_TEXTURE_COORD_ARRAY);

	glDisableClientState(GL_COLOR_ARRAY);
}
