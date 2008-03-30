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

#include "objects.h"

#include "keyboard.h"

#include "vector.h"
#include "types.h"
#include "debug.h"

#include <GL/glut.h>

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

void drawSquare(void)
{
	glBegin(GL_POLYGON);
	glNormal3f(0.0f, 0.0f, 1.0f);

	glTexCoord2f(0.0f, 1.0f);
	glVertex2f(-1.0f, -1.0f);

	glTexCoord2f(1.0f, 1.0f);
	glVertex2f(1.0f, -1.0f);

	glTexCoord2f(1.0f, 0.0f);
	glVertex2f(1.0f, 1.0f);

	glTexCoord2f(0.0f, 0.0f);
	glVertex2f(-1.0f, 1.0f);
	glEnd();
}

/*
 * data for icosahedron
 */

#define X 0.525731112119133606

#define Z 0.850650808352039932

static Vector3 gIsokaederVertices[12] = {
	{-X, 0.0, Z}, {X, 0.0, Z}, {-X, 0.0, -Z}, {X, 0.0, -Z},
	{0.0, Z, X}, {0.0, Z, -X}, {0.0, -Z, X}, {0.0, -Z, -X},
	{Z, X, 0.0}, {-Z, X, 0.0}, {Z, -X, 0.0}, {-Z, -X, 0.0}
};

static int gIsokaederIndices[20][3] = {
	{1, 4, 0}, {4, 9, 0}, {4, 5, 9}, {8, 5, 4}, {1, 8, 4},
	{1, 10, 8}, {10, 3, 8}, {8, 3, 5}, {3, 2, 5}, {3, 7, 2},
	{3, 10, 7}, {10, 6, 7}, {6, 11, 7}, {6, 0, 11}, {6, 1, 0},
	{10, 1, 6}, {11, 0, 9}, {2, 11, 9}, {5, 2, 9}, {11, 2, 7}
};

#define SUB_DIVS_DEPTH 3

/*
 * WARNING: CNT_BALL_TRIANGLES must be (20 * (4 ^ SUB_DIVS_DEPTH))
 */

#define CNT_BALL_TRIANGLES (20 * 64)
#define CNT_BALL_VERTICES (CNT_BALL_TRIANGLES * 3)

static int gCntBallVertices;
static Vector3 gBallVertices[CNT_BALL_VERTICES];
static Vector3 gBallColors[CNT_BALL_VERTICES];
static Vector3 gBallTexCoordsDefault[CNT_BALL_VERTICES];
static Vector3 gBallTexCoordsShader[CNT_BALL_VERTICES];

void subdiv(int depth, Vector3 tri[3], Vector3 hole, float s)
{
	Vector3 z = vector3(0.0f, 0.0f, 1.0f);
	int i;
	if (depth == 0)
	{
		float u0 = 0.5f * atan2(tri[0].x, tri[0].y) / PI + 0.5f;
		for (i = 0; i < 3; i++)
		{
			float light = 1.0f - acos(dot(tri[i], z)) / PI;

			Vector3 texCoordDefault;
			Vector3 texCoordShader;
			Vector3 color;

			texCoordShader = norm(sub(tri[i], hole));

			texCoordDefault.x = 0.5f * atan2(tri[i].x, tri[i].y) / PI + 0.5f;
			texCoordDefault.y = asin(tri[i].z) / PI + 0.5f;
			texCoordDefault.z = 0.0f;

			if (texCoordDefault.x - u0 > 0.5f)
			{
				texCoordDefault.x -= 1.0f;
			}
			else if (u0 - texCoordDefault.x > 0.5f)
			{
				texCoordDefault.x += 1.0f;
			}

			color.x = light;
			color.y = light;
			color.z = light;

			if (gCntBallVertices < CNT_BALL_VERTICES)
			{
				gBallVertices[gCntBallVertices] = tri[i];
				gBallColors[gCntBallVertices] = color;
				gBallTexCoordsDefault[gCntBallVertices] = texCoordDefault;
				gBallTexCoordsShader[gCntBallVertices] = texCoordShader;
				gCntBallVertices++;
			}
		}
	}
	else
	{
		Vector3 mid[3];
		Vector3 smallTri[3];

		for (i = 0; i < 3; i++)
		{
			mid[i] = norm(scale(0.5f, add(tri[i], tri[(i + 1) % 3])));
		}

		for (i = 0; i < 3; i++)
		{
			smallTri[0] = tri[i];
			smallTri[1] = mid[i];
			smallTri[2] = mid[(i + 2) % 3];

			subdiv(depth - 1, smallTri, tri[i], s);
		}

		subdiv(depth - 1, mid, tri[0], s);
	}
}

void initObjects(void)
{
	int i, j;

	gCntBallVertices = 0;

	for (i = 0; i < LENGTH(gIsokaederIndices); i++)
	{
		Vector3 tri[3];
		Vector3 mid = { 0.0f, 0.0f, 0.0f };

		for (j = 0; j < LENGTH(gIsokaederIndices[i]); j++)
		{
			tri[j] = gIsokaederVertices[gIsokaederIndices[i][j]];
			mid = add(mid, tri[j]);
		}

		mid = scale(1.0f / 3.0f, mid);

		subdiv(SUB_DIVS_DEPTH, tri, tri[0], 1.0f / len(sub(tri[0], mid)));
	}
}

void drawBallObject(int shader)
{
	glVertexPointer(3, GL_FLOAT, 0, gBallVertices);
	glEnableClientState(GL_VERTEX_ARRAY);

#if 0
	glColorPointer(3, GL_FLOAT, 0, gBallColors);
	glEnableClientState(GL_COLOR_ARRAY);
#endif

	if (shader)
	{
		glTexCoordPointer(3, GL_FLOAT, 0, gBallTexCoordsShader);
	}
	else
	{
		glTexCoordPointer(3, GL_FLOAT, 0, gBallTexCoordsDefault);
	}
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);

	glNormalPointer(GL_FLOAT, 0, gBallVertices);
	glEnableClientState(GL_NORMAL_ARRAY);

	glDrawArrays(GL_TRIANGLES, 0, gCntBallVertices);

	glDisableClientState(GL_VERTEX_ARRAY);
#if 0
	glDisableClientState(GL_COLOR_ARRAY);
#endif
	glDisableClientState(GL_TEXTURE_COORD_ARRAY);
	glDisableClientState(GL_NORMAL_ARRAY);
}

/* ball explosion */

/*
 * T1 < T2
 */
#define T1 1.0f
#define T2 5.0f

#define PARTS_TOGETHER 16

typedef struct
{
	Vector3 vertices[PARTS_TOGETHER * 3];

	Vector3 pos;
	Vector3 offset;
	Vector3 speed;

	Vector3 rotation;
	Vector3 rotSpeed;
} Fragment;

Fragment gFragments[CNT_BALL_TRIANGLES / PARTS_TOGETHER];

Vector3 gStartPos;
Vector3 gStartSpeed;

Vector3 gEndPos;
Vector3 gEndSpeed;

float gExplosionTime;
float gMaxExplosionTime;

float randFloat(void)
{
	return ((float) rand() / RAND_MAX) * 2.0f - 1.0f;
}

/*
 * WARNING: must be caled befor each new explosion
 */
void initExplosion(Vector3 startPos, Vector3 startSpeed, Vector3 endPos,
									 Vector3 endSpeed)
{
	int i;
	int j;
	int v;

	gExplosionTime = 0.0;
	gMaxExplosionTime = 5.0;

	gStartPos = startPos;
	gStartSpeed = startSpeed;
	gEndPos = endPos;
	gEndSpeed = endSpeed;

	for (i = 0, v = 0; i < LENGTH(gFragments);
			 i++, v += LENGTH(gFragments[i].vertices))
	{
		Vector3 mid = { 0.0f, 0.0f, 0.0f };

		for (j = 0; j < LENGTH(gFragments[i].vertices); j++)
		{
			mid = add(mid, gBallVertices[v + j]);
		}

		mid = scale(1.0f / LENGTH(gFragments[i].vertices), mid);

		for (j = 0; j < LENGTH(gFragments[i].vertices); j++)
		{
			gFragments[i].vertices[j] = sub(gBallVertices[v + j], mid);
		}

		gFragments[i].pos.x = 0.0f;
		gFragments[i].pos.y = 0.0f;
		gFragments[i].pos.z = 0.0f;

		gFragments[i].offset = mid;
		gFragments[i].speed = scale(200.0f, mid);

		gFragments[i].rotation.x = 0.0f;
		gFragments[i].rotation.y = 0.0f;
		gFragments[i].rotation.z = 0.0f;

		gFragments[i].rotSpeed.x = 360.0f * randFloat();
		gFragments[i].rotSpeed.y = 360.0f * randFloat();
		gFragments[i].rotSpeed.z = 360.0f * randFloat();
	}
}

float smallestError(float x)
{
	x = fmod(x, 360.0f);

	if (x > 180.0f)
	{
		x -= 360.0f;
	}
	else if (x < -180.0f)
	{
		x += 360.0f;
	}

	return x;
}

/*
 * interpolate explosion's position to destination
 */
int updateExplosion(float interval, Vector3 * speed, Vector3 * pos)
{
	int i;

	float t = gExplosionTime / gMaxExplosionTime;

	float b0 = 2 * t * t * t - 3 * t * t + 1;
	float b1 = -2 * t * t * t + 3 * t * t;
	float b2 = t * t * t - 2 * t * t + t;
	float b3 = t * t * t - t * t;

	pos->x = 0;
	pos->y = 0;
	pos->z = 0;

	*pos = add(*pos, scale(b0, gStartPos));
	*pos = add(*pos, scale(b1, gEndPos));
	*pos = add(*pos, scale(b2, gStartSpeed));
	*pos = add(*pos, scale(b3, gEndSpeed));

	for (i = 0; i < LENGTH(gFragments); i++)
	{
		Vector3 rotError;

		/* position */
		gFragments[i].pos.x = 0.0f;
		gFragments[i].pos.y = 0.0f;
		gFragments[i].pos.z = 0.0f;

		gFragments[i].pos =
			add(gFragments[i].pos, scale(b0 + b1, gFragments[i].offset));
		gFragments[i].pos = add(gFragments[i].pos, scale(b2, gFragments[i].speed));

		/* rotation */
		gFragments[i].rotSpeed =
			sub(gFragments[i].rotSpeed, scale(T1 * interval, gFragments[i].rotSpeed));

		rotError = sub(gFragments[i].rotSpeed, gFragments[i].rotation);

		rotError.x = smallestError(rotError.x);
		rotError.y = smallestError(rotError.y);
		rotError.z = smallestError(rotError.z);

		gFragments[i].rotation =
			add(gFragments[i].rotation, scale(T2 * interval, rotError));
	}

	gExplosionTime += interval;

	if (wasKeyPressed(KEY_ENTER) && gMaxExplosionTime - gExplosionTime > 1.0f)
	{
		gExplosionTime = gMaxExplosionTime - 1.0f;

		for (i = 0; i < LENGTH(gFragments); i++)
		{
			gFragments[i].rotSpeed = vector3(0.0f, 0.0f, 0.0f);
			gFragments[i].rotation = vector3(0.0f, 0.0f, 0.0f);
		}
	}

	return gExplosionTime >= gMaxExplosionTime;
}

void drawExplosion(int shader)
{
	int i;
	int j;
	int v;

	glLightModeli(GL_LIGHT_MODEL_TWO_SIDE, 1);
	glDisable(GL_CULL_FACE);

	for (i = 0, v = 0; i < LENGTH(gFragments);
			 i++, v += LENGTH(gFragments[i].vertices))
	{
		glPushMatrix();

		glTranslatef(gFragments[i].pos.x, gFragments[i].pos.y, gFragments[i].pos.z);

		glRotatef(gFragments[i].rotation.x, 1.0f, 0.0f, 0.0f);
		glRotatef(gFragments[i].rotation.y, 0.0f, 1.0f, 0.0f);
		glRotatef(gFragments[i].rotation.z, 0.0f, 0.0f, 1.0f);

		glBegin(GL_TRIANGLES);
		for (j = 0; j < LENGTH(gFragments[i].vertices); j++)
		{
			glNormal3fv(&gBallVertices[v + j].x);

			if (shader)
			{
				glTexCoord3fv(&gBallTexCoordsShader[v + j].x);
			}
			else
			{
				glTexCoord3fv(&gBallTexCoordsDefault[v + j].x);
			}

			glVertex3fv(&gFragments[i].vertices[j].x);
		}
		glEnd();

		glPopMatrix();
	}

	glEnable(GL_CULL_FACE);
	glLightModeli(GL_LIGHT_MODEL_TWO_SIDE, 0);
}
