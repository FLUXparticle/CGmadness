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

#include "field.hpp"

#include "level.hpp"
#include "camera.hpp"

#include "ball.hpp"
#include "features.hpp"

#include "math/Vector2.hpp"
#include "functions.hpp"

#include "types.hpp"

#include <GL/glew.h>
#include <GL/gl.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#define WITHOUT_DEPTH_TEST 0
#define WITH_STENCIL_TEST 0
#define TWO_PASS 0

Vector3 *sgVertices;
Vector3 *sgNormals;

static Vector2 gDefaultTexCoord;
static Vector2 gDefaultLightMapCoord;
static Vector3 gDefaultNormal;
static Color4 gDefaultColor;

static Vector2 *gTexCoords;
static Vector2 *gLightMapCoords;
static Color4 *gColors;
static unsigned int gVBuffers[6];

static Vector3 *gBallShadowCoords;

static GLuint gWhiteTexture;
static GLuint gBallShadow;

static int gCntVertices;
static int gMaxPlates;
static int gMaxQuads;
static int gMaxVertices;

static int gCntCameraViewIndices = 0;
static int *gCameraViewIndices;

static int gCntBallReflectionIndices = 0;
static int *gBallReflectionIndices;

static int *gIndexVertices;

static Vector3 gBallPosition;

static void setColor(Color4 col)
{
	gDefaultColor = col;
}

static void setTexCoord(Vector2 uv)
{
	gDefaultTexCoord = uv;
}

static void setLightMapCoord(Vector2 uv)
{
	gDefaultLightMapCoord = uv;
}

static void setNormal(Vector3 n)
{
	gDefaultNormal = n;
}

static void addVertex(Vector3 v)
{
	gTexCoords[gCntVertices] = gDefaultTexCoord;
	gLightMapCoords[gCntVertices] = gDefaultLightMapCoord;
	sgNormals[gCntVertices] = gDefaultNormal;
	gColors[gCntVertices] = gDefaultColor;

	sgVertices[gCntVertices] = v;

	gCntVertices++;
}

void addSquare(const Square * square)
{
	int i;
	setNormal(square->normal);

	for (i = 0; i < 4; i++)
	{
		setTexCoord(square->texcoord[i]);
		setLightMapCoord(square->lightmap[i]);
		addVertex(square->vertices[i]);
	}
}

void getVertIndex(int x, int y, int *start, int *end)
{
	if (x >= 0 && x < sgLevel.size.x && y >= 0 && y < sgLevel.size.y)
	{
		int index = y * sgLevel.size.x + x;
		*start = gIndexVertices[index];
		index++;
		if (index < gMaxPlates)
		{
			*end = gIndexVertices[index];
		}
		else
		{
			*end = gCntVertices;
		}
	}
	else
	{
		*start = 0;
		*end = 0;
	}
}

void setSquareColor(int q, Color4 col)
{
	int i;

	for (i = 0; i < 4; i++)
	{
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

#define bufferdata(x) glBufferDataARB(GL_ARRAY_BUFFER_ARB, sizeof(*(x)) * gCntVertices, (x), GL_STATIC_DRAW_ARB);

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

				ballShadowData[z][y][x] =
					1.0f - (fz / d) / (1.0f + sqr(d) / sqr(BALL_RADIUS));
			}
		}
	}

	glGenTextures(1, &gBallShadow);
	glBindTexture(GL_TEXTURE_3D, gBallShadow);

	glTexImage3D(GL_TEXTURE_3D, 0, GL_LUMINANCE, SAMPLES_XY, SAMPLES_XY,
							 SAMPLES_Z, 0, GL_LUMINANCE, GL_FLOAT, &ballShadowData[0][0][0]);

	glTexParameterf(GL_TEXTURE_3D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameterf(GL_TEXTURE_3D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameterf(GL_TEXTURE_3D, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

	glTexParameterf(GL_TEXTURE_3D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_3D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
}

void initGameField(void)
{
	static Color4 green = { 0.0f, 1.0f, 0.0f, 1.0f };
	static Color4 blue = { 0.0f, 0.0f, 1.0f, 1.0f };
	static Color4 white = { 1.0f, 1.0f, 1.0f, 1.0f };

	int x;
	int y;
	int i;
	int index = 0;

	glGenTextures(1, &gWhiteTexture);

	glBindTexture(GL_TEXTURE_2D, gWhiteTexture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_LUMINANCE, 1, 1, 0, GL_LUMINANCE, GL_FLOAT,
							 &white.r);

	initBallShadow();

	/* init level stuff */
	gMaxPlates = sgLevel.size.x * sgLevel.size.y;
	gMaxQuads = 0;

	for (y = 0; y < sgLevel.size.y; y++)
	{
		for (x = 0; x < sgLevel.size.x; x++)
		{
			gMaxQuads++;

			for (i = 0; i < 4; i++)
			{
				SideFace face;

				getSideFace(x, y, i, &face);

				gMaxQuads += face.cntSquares;
			}
		}
	}

	gMaxVertices = 4 * gMaxQuads;

	sgVertices = new Vector3[gMaxVertices];
	sgNormals = new Vector3[gMaxVertices];
	gIndexVertices = new int[gMaxPlates];
	gTexCoords = new Vector2[gMaxVertices];
	gLightMapCoords = new Vector2[gMaxVertices];
	gColors = new Color4[gMaxVertices];

	gBallShadowCoords = new Vector3[gMaxVertices];

	gCntVertices = 0;

	setColor(white);

	for (y = 0; y < sgLevel.size.y; y++)
	{
		for (x = 0; x < sgLevel.size.x; x++)
		{
			Square square;

			gIndexVertices[index++] = gCntVertices;

			getRoofSquare(x, y, &square);

			addSquare(&square);

			for (i = 0; i < 4; i++)
			{
				SideFace face;
				int k;

				getSideFace(x, y, i, &face);

				for (k = 0; k < face.cntSquares; k++)
				{
					addSquare(&face.squares[k]);
				}
			}
		}
	}

	setRoofColor(sgLevel.start.x, sgLevel.start.y, green);
	setRoofColor(sgLevel.finish.x, sgLevel.finish.y, blue);

	if (hasVertexbuffer())
	{
		glGenBuffersARB(LENGTH(gVBuffers), gVBuffers);

		glBindBufferARB(GL_ARRAY_BUFFER_ARB, gVBuffers[0]);
		bufferdata(sgVertices);

		glBindBufferARB(GL_ARRAY_BUFFER_ARB, gVBuffers[1]);
		bufferdata(sgNormals);

		glBindBufferARB(GL_ARRAY_BUFFER_ARB, gVBuffers[2]);
		bufferdata(gLightMapCoords);

		glBindBufferARB(GL_ARRAY_BUFFER_ARB, gVBuffers[3]);
		bufferdata(gTexCoords);

		glBindBufferARB(GL_ARRAY_BUFFER_ARB, gVBuffers[4]);
		bufferdata(gColors);

		glBindBufferARB(GL_ARRAY_BUFFER_ARB, gVBuffers[5]);
		glBufferDataARB(GL_ARRAY_BUFFER_ARB, sizeof(Vector3) * gCntVertices,
										(gBallShadowCoords), GL_DYNAMIC_DRAW);

		glBindBufferARB(GL_ARRAY_BUFFER_ARB, 0);
	}

	printf("gMaxVertices: %d\n", gMaxVertices);
	printf("gCntVertices: %d\n", gCntVertices);

	gCntCameraViewIndices = 0;
	gCameraViewIndices = new int[gCntVertices];
	gBallReflectionIndices = new int[gCntVertices];
}

void destroyGameField(void)
{
	delete[] sgVertices;
	delete[] sgNormals;

	delete[] gIndexVertices;
	delete[] gCameraViewIndices;
	delete[] gBallReflectionIndices;
	delete[] gTexCoords;
	delete[] gLightMapCoords;
	delete[] gColors;

	delete[] gBallShadowCoords;

	gMaxVertices = 0;
	gCameraViewIndices = 0;
	gCntBallReflectionIndices = 0;
}

/*
 * render from close to far
 */
void bsp(int startX, int startY, int sizeX, int sizeY, int viewX, int viewY,
				 Vector3 camera, int farToClose, int *indices, int *index)
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
		int i;

		getVertIndex(startX, startY, &start, &end);

		for (q = start; q < end; q += 4)
		{
			/*
			 * the top square must always be drawn, because this function
			 * is not called if only the height of the camera changes.
			 * Fortunately it is always the first square in the array range.
			 * WARNING: be aware of this if you change the order of sqaures.
			 */
			if (q == start || dot(sgNormals[q], sub(camera, sgVertices[q])) >= 0)
			{
				for (i = 0; i < 4; i++)
				{
					indices[(*index)++] = q + i;
				}
			}
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
			bsp(startX1, startY1, sizeX1, sizeY1, viewX, viewY, camera, farToClose,
					indices, index);
			bsp(startX2, startY2, sizeX2, sizeY2, viewX, viewY, camera, farToClose,
					indices, index);
		}
		else
		{
			bsp(startX2, startY2, sizeX2, sizeY2, viewX, viewY, camera, farToClose,
					indices, index);
			bsp(startX1, startY1, sizeX1, sizeY1, viewX, viewY, camera, farToClose,
					indices, index);
		}
	}
}

void updateGameField(const Ball& ball)
{
	gBallPosition = ball.pos();
	
	static int lastMX = 0;
	static int lastMY = 0;

	int mx = (int) floor(sgCamera.x - sgLevel.origin.x);
	int my = (int) floor(sgCamera.y - sgLevel.origin.y);

	if (gCntCameraViewIndices == 0 || !(mx == lastMX && my == lastMY))
	{
		gCntCameraViewIndices = 0;
		if (gMaxVertices > 0)
		{
			bsp(0, 0, sgLevel.size.x, sgLevel.size.y, mx, my, sgCamera, 0,
					gCameraViewIndices, &gCntCameraViewIndices);

			lastMX = mx;
			lastMY = my;
		}
	}

	if (!hasBallShadowShader() && useBallShadow())
	{
		int q;

		for (q = 0; q < gCntVertices; q += 4)
		{
			Vector3 vz = sgNormals[q];
			Vector3 vx = norm(sub(sgVertices[q + 1], sgVertices[q]));
			Vector3 vy = norm(cross(vx, vz));

			int i;

			for (i = 0; i < 4; i++)
			{
				Vector3 vertex = sgVertices[q + i];
				Vector3 d = sub(gBallPosition, vertex);

				float x = dot(vx, d) / MAX_XY + 0.5f;
				float y = dot(vy, d) / MAX_XY + 0.5f;
				float z = dot(vz, d) / MAX_Z;

				gBallShadowCoords[q + i] = Vector3(x, y, z);
			}
		}
	}

	if (ball.useBallReflection())
	{
		static int lastBX = 0;
		static int lastBY = 0;

		int bx = (int) floor(gBallPosition.x - sgLevel.origin.x);
		int by = (int) floor(gBallPosition.y - sgLevel.origin.y);

		if (gCntBallReflectionIndices == 0 || !(bx == lastBX && by == lastBY))
		{
			gCntBallReflectionIndices = 0;
			if (gMaxVertices > 0)
			{
				bsp(0, 0, sgLevel.size.x, sgLevel.size.y, bx, by, gBallPosition, 1,
						gBallReflectionIndices, &gCntBallReflectionIndices);

				lastBX = bx;
				lastBY = by;
			}
		}
	}
}

void drawGameField(bool ballReflection)
{
	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_NORMAL_ARRAY);

	glClientActiveTextureARB(GL_TEXTURE0);
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);

	glClientActiveTextureARB(GL_TEXTURE1);
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

		glClientActiveTextureARB(GL_TEXTURE1);
		glBindBufferARB(GL_ARRAY_BUFFER_ARB, gVBuffers[3]);
		glTexCoordPointer(2, GL_FLOAT, 0, NULL);

		glBindBufferARB(GL_ARRAY_BUFFER_ARB, gVBuffers[4]);
		glColorPointer(4, GL_FLOAT, 0, NULL);
	}
	else
	{
		glVertexPointer(3, GL_FLOAT, 0, sgVertices);

		glNormalPointer(GL_FLOAT, 0, sgNormals);

		glClientActiveTextureARB(GL_TEXTURE0);
		glTexCoordPointer(2, GL_FLOAT, 0, gLightMapCoords);

		glClientActiveTextureARB(GL_TEXTURE1);
		glTexCoordPointer(2, GL_FLOAT, 0, gTexCoords);

		glColorPointer(4, GL_FLOAT, 0, gColors);
	}

	glActiveTexture(GL_TEXTURE0);
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, sgLevel.lightMap);

	glActiveTexture(GL_TEXTURE1);
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, sgLevel.borderTexture);

	if (useBallShadow())
	{
		if (hasBallShadowShader())
		{
			glUseProgram(sgBallShadowShader);

			glUniform3fv(glGetUniformLocation(sgBallShadowShader, "ball"), 1,
									 &gBallPosition.x);
			glUniform1i(glGetUniformLocation(sgBallShadowShader, "tex0"), 0);
			glUniform1i(glGetUniformLocation(sgBallShadowShader, "tex1"), 1);
		}
		else
		{
			glActiveTexture(GL_TEXTURE2);
			glEnable(GL_TEXTURE_3D);
			glBindTexture(GL_TEXTURE_3D, gBallShadow);

			glClientActiveTextureARB(GL_TEXTURE2);
			glEnableClientState(GL_TEXTURE_COORD_ARRAY);
			if (hasVertexbuffer())
			{
				void *data;

				glBindBufferARB(GL_ARRAY_BUFFER_ARB, gVBuffers[5]);

				data = glMapBufferARB(GL_ARRAY_BUFFER_ARB, GL_WRITE_ONLY_ARB);

				memcpy(data, gBallShadowCoords, sizeof(Vector3) * gCntVertices);

				glUnmapBufferARB(GL_ARRAY_BUFFER_ARB);

				glTexCoordPointer(3, GL_FLOAT, 0, NULL);
			}
			else
			{
				glTexCoordPointer(3, GL_FLOAT, 0, gBallShadowCoords);
			}
		}
	}

	if (ballReflection)
	{
		glDrawElements(GL_QUADS, gCntBallReflectionIndices, GL_UNSIGNED_INT,
									 gBallReflectionIndices);
	}
	else
	{
		glDrawElements(GL_QUADS, gCntCameraViewIndices, GL_UNSIGNED_INT,
									 gCameraViewIndices);
	}

	if (useBallShadow())
	{
		if (hasShader())
		{
			glUseProgram(0);
		}
		else
		{
			glActiveTexture(GL_TEXTURE2);
			glDisable(GL_TEXTURE_3D);

			glClientActiveTextureARB(GL_TEXTURE2);
			glDisableClientState(GL_TEXTURE_COORD_ARRAY);
		}
	}

	glActiveTexture(GL_TEXTURE1);
	glDisable(GL_TEXTURE_2D);

	glActiveTexture(GL_TEXTURE0);
	glDisable(GL_TEXTURE_2D);

#if 0
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	glLineWidth(5.0f);

	glColor3f(0.0f, 0.0f, 0.0f);
	glDrawElements(GL_QUADS, gCntCameraViewIndices, GL_UNSIGNED_INT,
								 ballReflection ? gBallReflectionIndices : gCameraViewIndices);

	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
#endif

	if (hasVertexbuffer())
	{
		glBindBufferARB(GL_ARRAY_BUFFER_ARB, 0);
	}

	glDisableClientState(GL_VERTEX_ARRAY);
	glDisableClientState(GL_NORMAL_ARRAY);

	glClientActiveTextureARB(GL_TEXTURE2);
	glDisableClientState(GL_TEXTURE_COORD_ARRAY);

	glClientActiveTextureARB(GL_TEXTURE1);
	glDisableClientState(GL_TEXTURE_COORD_ARRAY);

	glClientActiveTextureARB(GL_TEXTURE0);
	glDisableClientState(GL_TEXTURE_COORD_ARRAY);

	glDisableClientState(GL_COLOR_ARRAY);
}
