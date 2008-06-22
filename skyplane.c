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

#include "skyplane.h"

#include "features.h"
#include "functions.h"
#include "vector.h"
#include "color.h"
#include "texture.h"

#include "debug.h"

#include <GL/glew.h>

#include <math.h>

#define PLANET_RADIUS 2000.0f
#define ATMOSPHERE_RADIUS 180.0f
#define hTile 3
#define vTile 3
#define ALPHA 0.85f
#define DIVS 3
#define exponentialFadeout 0

#define NUM_OF_VERTS ((DIVS + 1) * (DIVS + 1))
#define NUM_OF_INDICES (DIVS * DIVS * 2 * 3)

static int texID;

static Vector3 gPlaneVerts[NUM_OF_VERTS];
static Vector2 gPlaneTexCoords[NUM_OF_VERTS];
static Color4 gPlaneColors[NUM_OF_VERTS];
static GLuint gIndices[NUM_OF_INDICES];

void initSkyplane(void)
{
	float planeSize = 2.0f * PLANET_RADIUS / sqrt(2.0);
	float delta = planeSize / DIVS;
	float alpharadius_squared = sqr(planeSize * ALPHA * 0.5f);
	float planetradius_squared = sqr(PLANET_RADIUS);

	int i;
	int j;
	int index = 0;

	texID = loadTexture("data/clouds.tga", 1);

	for (i = 0; i <= DIVS; i++)
	{
		for (j = 0; j <= DIVS; j++)
		{
			int idx = i * (DIVS + 1) + j;

			float xdist = (j * delta) - (0.5f * planeSize);
			float ydist = (i * delta) - (0.5f * planeSize);

			float xheight = sqr(xdist);
			float yheight = sqr(ydist);
			float height = (xheight + yheight) / planetradius_squared;

			float zdist = (1.0f - height) * ATMOSPHERE_RADIUS;

			float u = hTile * ((float) j / DIVS);
			float v = vTile * ((float) i / DIVS);

			float alpha = (xheight + yheight) / alpharadius_squared;

#if (exponentialFadeout)
			alpha = sqr(alpha) * alpha;
#endif

			alpha = max(0.0f, 1.0f - alpha);

			gPlaneVerts[idx] = vector3(xdist, ydist, zdist);
			gPlaneTexCoords[idx] = vector2(u, v);
			gPlaneColors[idx] = color4(1.0f, 1.0f, 1.0f, alpha);
		}
	}

	for (i = 0; i < DIVS; i++)
	{
		for (j = 0; j < DIVS; j++)
		{
			int startVert = i * (DIVS + 1) + j;

			gIndices[index++] = startVert;
			gIndices[index++] = startVert + DIVS + 1;
			gIndices[index++] = startVert + 1;

			gIndices[index++] = startVert + 1;
			gIndices[index++] = startVert + DIVS + 1;
			gIndices[index++] = startVert + DIVS + 2;
		}
	}
}

void drawSkyplane(void)
{
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, texID);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);
	glEnableClientState(GL_COLOR_ARRAY);

	glVertexPointer(3, GL_FLOAT, 0, gPlaneVerts);
	glTexCoordPointer(2, GL_FLOAT, 0, gPlaneTexCoords);
	glColorPointer(4, GL_FLOAT, 0, gPlaneColors);

	glDrawElements(GL_TRIANGLES, NUM_OF_INDICES, GL_UNSIGNED_INT, gIndices);

	glDisableClientState(GL_VERTEX_ARRAY);
	glDisableClientState(GL_TEXTURE_COORD_ARRAY);
	glDisableClientState(GL_COLOR_ARRAY);

	glDisable(GL_BLEND);
	glDisable(GL_TEXTURE_2D);
}
