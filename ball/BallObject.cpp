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

#include "BallObject.hpp"

#include "macros.hpp"

#include <GL/glew.h>

#include <math.h>

Vector3 BallObject::gBallVertices[CNT_BALL_VERTICES];
Vector3 BallObject::gBallTexCoordsDefault[CNT_BALL_VERTICES];
Vector3 BallObject::gBallTexCoordsShader[CNT_BALL_VERTICES];

int BallObject::gCntBallVertices;

/*
 * data for icosahedron
 */

#define X 0.525731112119133606

#define Z 0.850650808352039932

static Vector3 gIsokaederVertices[12] =
{ Vector3(-X, 0.0, Z), Vector3(X, 0.0, Z), Vector3(-X, 0.0, -Z),
		Vector3(X, 0.0, -Z), Vector3(0.0, Z, X), Vector3(0.0, Z, -X), Vector3(0.0,
		-Z, X), Vector3(0.0, -Z, -X), Vector3(Z, X, 0.0), Vector3(-Z, X, 0.0),
		Vector3(Z, -X, 0.0), Vector3(-Z, -X, 0.0) };

static int gIsokaederIndices[20][3] =
{
{ 1, 4, 0 },
{ 4, 9, 0 },
{ 4, 5, 9 },
{ 8, 5, 4 },
{ 1, 8, 4 },
{ 1, 10, 8 },
{ 10, 3, 8 },
{ 8, 3, 5 },
{ 3, 2, 5 },
{ 3, 7, 2 },
{ 3, 10, 7 },
{ 10, 6, 7 },
{ 6, 11, 7 },
{ 6, 0, 11 },
{ 6, 1, 0 },
{ 10, 1, 6 },
{ 11, 0, 9 },
{ 2, 11, 9 },
{ 5, 2, 9 },
{ 11, 2, 7 } };

void BallObject::subdiv(int depth, Vector3 tri[3], Vector3 hole, float s)
{
	Vector3 z = Vector3(0.0f, 0.0f, 1.0f);
	int i;
	if (depth == 0)
	{
		float u0 = 0.5f * atan2(tri[0].x, tri[0].y) / M_PI+ 0.5f;
		for (i = 0; i < 3; i++)
		{
			Vector3 texCoordDefault;
			Vector3 texCoordShader;
			Vector3 color;

			texCoordShader = norm(sub(tri[i], hole));

			texCoordDefault.x = 0.5f * atan2(tri[i].x, tri[i].y) / M_PI+ 0.5f;
			texCoordDefault.y = asin(tri[i].z) / M_PI+ 0.5f;
			texCoordDefault.z = 0.0f;

			if (texCoordDefault.x - u0 > 0.5f)
			{
				texCoordDefault.x -= 1.0f;
			}
			else if (u0 - texCoordDefault.x> 0.5f)
			{
				texCoordDefault.x += 1.0f;
			}

			if (gCntBallVertices < CNT_BALL_VERTICES)
			{
				gBallVertices[gCntBallVertices] = tri[i];
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

void BallObject::draw(Vector3 ballTexCoords[CNT_BALL_VERTICES]) const
{
	glVertexPointer(3, GL_FLOAT, 0, gBallVertices);
	glEnableClientState(GL_VERTEX_ARRAY);

	glTexCoordPointer(3, GL_FLOAT, 0, ballTexCoords);

	glEnableClientState(GL_TEXTURE_COORD_ARRAY);

	glNormalPointer(GL_FLOAT, 0, gBallVertices);
	glEnableClientState(GL_NORMAL_ARRAY);
	{
		glDrawArrays(GL_TRIANGLES, 0, gCntBallVertices);
	}
	glDisableClientState(GL_VERTEX_ARRAY);
	glDisableClientState(GL_TEXTURE_COORD_ARRAY);
	glDisableClientState(GL_NORMAL_ARRAY);
}

void BallObject::init()
{
	gCntBallVertices = 0;

	for (unsigned int i = 0; i < LENGTH(gIsokaederIndices); i++)
	{
		Vector3 tri[3];
		Vector3 mid(0.0f, 0.0f, 0.0f);

		for (unsigned int j = 0; j < LENGTH(gIsokaederIndices[i]); j++)
		{
			tri[j] = gIsokaederVertices[gIsokaederIndices[i][j]];
			mid = add(mid, tri[j]);
		}

		mid = scale(1.0f / 3.0f, mid);

		subdiv(SUB_DIVS_DEPTH, tri, tri[0], 1.0f / len(sub(tri[0], mid)));
	}
}

void BallObject::drawDefault() const
{
	draw(gBallTexCoordsDefault);
}

void BallObject::drawShader() const
{
	draw(gBallTexCoordsShader);
}

BallObject::BallObject()
{
	// empty
}

BallObject::~BallObject()
{
	// empty
}
