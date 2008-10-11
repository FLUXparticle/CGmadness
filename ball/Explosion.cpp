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

#include "Explosion.hpp"

#include "hw/keyboard.hpp"

#include "macros.hpp"

#include GL_H

#include <stdlib.h>
#include <math.h>

/*
 * T1 < T2
 */
#define T1 1.0f
#define T2 5.0f

float randFloat(void)
{
	return ((float) rand() / RAND_MAX) * 2.0f- 1.0f;
}

/*
 * WARNING: must be caled befor each new explosion
 */
void Explosion::init(const Vector3& startPos, const Vector3& startSpeed,
		const Vector3& endPos, const Vector3& endSpeed)
{
	gExplosionTime = 0.0;
	gMaxExplosionTime = 5.0;

	gStartPos = startPos;
	gStartSpeed = startSpeed;
	gEndPos = endPos;
	gEndSpeed = endSpeed;

	for (unsigned int i = 0, v = 0; i < LENGTH(gFragments); i++, v
			+= LENGTH(gFragments[i].vertices))
	{
		Vector3 mid(0.0f, 0.0f, 0.0f);

		for (unsigned int j = 0; j < LENGTH(gFragments[i].vertices); j++)
		{
			mid = add(mid, gBallVertices[v + j]);
		}

		mid = scale(1.0f / LENGTH(gFragments[i].vertices), mid);

		for (unsigned int j = 0; j < LENGTH(gFragments[i].vertices); j++)
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
bool Explosion::update(float interval, const Vector3& speed, Vector3& pos)
{
	float t = gExplosionTime / gMaxExplosionTime;

	float b0 = 2 * t * t * t - 3 * t * t + 1;
	float b1 = -2 * t * t * t + 3 * t * t;
	float b2 = t * t * t - 2 * t * t + t;
	float b3 = t * t * t - t * t;

	pos = scale(b0, gStartPos) + scale(b1, gEndPos) + scale(b2, gStartSpeed)
			+ scale(b3, gEndSpeed);

	for (unsigned int i = 0; i < LENGTH(gFragments); i++)
	{
		Vector3 rotError;

		/* position */
		gFragments[i].pos.x = 0.0f;
		gFragments[i].pos.y = 0.0f;
		gFragments[i].pos.z = 0.0f;

		gFragments[i].pos =add(gFragments[i].pos, scale(b0 + b1,
				gFragments[i].offset));
		gFragments[i].pos = add(gFragments[i].pos, scale(b2, gFragments[i].speed));

		/* rotation */
		gFragments[i].rotSpeed =sub(gFragments[i].rotSpeed,
				scale(T1 * interval, gFragments[i].rotSpeed));

		rotError = sub(gFragments[i].rotSpeed, gFragments[i].rotation);

		rotError.x = smallestError(rotError.x);
		rotError.y = smallestError(rotError.y);
		rotError.z = smallestError(rotError.z);

		gFragments[i].rotation =add(gFragments[i].rotation,
				scale(T2 * interval, rotError));
	}

	gExplosionTime += interval;

	if (wasKeyPressed(KEY_ENTER) && gMaxExplosionTime - gExplosionTime > 1.0f)
	{
		gExplosionTime = gMaxExplosionTime - 1.0f;

		for (unsigned int i = 0; i < LENGTH(gFragments); i++)
		{
			gFragments[i].rotSpeed = Vector3(0.0f, 0.0f, 0.0f);
			gFragments[i].rotation = Vector3(0.0f, 0.0f, 0.0f);
		}
	}

	return gExplosionTime >= gMaxExplosionTime;
}

void Explosion::draw(Vector3 ballTexCoords[CNT_BALL_VERTICES]) const
{
	glLightModeli(GL_LIGHT_MODEL_TWO_SIDE, 1);
	glDisable(GL_CULL_FACE);

	for (unsigned int i = 0, v = 0; i < LENGTH(gFragments); i++, v
			+= LENGTH(gFragments[i].vertices))
	{
		glPushMatrix();

		glTranslatef(gFragments[i].pos.x, gFragments[i].pos.y, gFragments[i].pos.z);

		glRotatef(gFragments[i].rotation.x, 1.0f, 0.0f, 0.0f);
		glRotatef(gFragments[i].rotation.y, 0.0f, 1.0f, 0.0f);
		glRotatef(gFragments[i].rotation.z, 0.0f, 0.0f, 1.0f);

		glBegin(GL_TRIANGLES);
		for (unsigned int j = 0; j < LENGTH(gFragments[i].vertices); j++)
		{
			glNormal3fv(&gBallVertices[v + j].x);
			glTexCoord3fv(&ballTexCoords[v + j].x);
			glVertex3fv(&gFragments[i].vertices[j].x);
		}
		glEnd();

		glPopMatrix();
	}

	glEnable(GL_CULL_FACE);
	glLightModeli(GL_LIGHT_MODEL_TWO_SIDE, 0);
}

void Explosion::drawDefault() const
{
	draw(gBallTexCoordsDefault);
}

void Explosion::drawShader() const
{
	draw(gBallTexCoordsShader);
}

Explosion::Explosion()
{
	// empty
}

Explosion::~Explosion()
{
	// empty
}

