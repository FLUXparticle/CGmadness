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

#include "Main.hpp"
#include "hw/features.hpp"
#include "Ball.hpp"

#include "ColorStack.hpp"

#include "level/level.hpp"
#include "texture.hpp"
#include "process/Game.hpp"
#include "hw/keyboard.hpp"
#include "field.hpp"
#include "lightmap.hpp"

#include "functions.hpp"

#include GL_H

#include <math.h>
#include <string.h>

#define SHOW_COLLISION_QUADS 0

#define MOVE_FORCE 5.0f
#define GRAVITY 9.81f
#define JUMP_FORCE (50.0f * MOVE_FORCE)
#define DAMPING 0.99f
#define ELASTICITY 0.5f

int Ball::sTextureBall = 0;

void Ball::init()
{
	sTextureBall = loadTexture("data/ball.tga", false);
}

bool Ball::hasBallTexture(void)
{
	return sTextureBall >= 0;
}

Ball::Ball() :
	mMass(1.0f),
	mRadius(BALL_RADIUS)
{
	mOrientation = mkQuaternion(0.0f, Vector3(0.0f, 0.0f, 1.0f));
}

Ball::~Ball()
{
  // empty
}

void Ball::reset()
{
	Square roofSquare;

	getRoofSquare(sgLevel.start.x, sgLevel.start.y, &roofSquare);

	mPos.x = roofSquare.mid.x;
	mPos.y = roofSquare.mid.y;
	mPos.z = getMaxZValue(&roofSquare) + 2.5f;

	mVelocity = Vector3(0.0f, 0.0f, 0.0f);
	mAngularRate = Vector3(0.0f, 0.0f, 0.0f);

	mIsBallInPieces = false;
	mHasBallHitGoal = false;
}

void Ball::push(const Vector3& direction)
{
	mPushDirection = norm(direction);
}

void Ball::update(float interval)
{
	if (!mIsBallInPieces)
	{
		animateBall(interval);
	}
	else if (mExplosion.update(interval, mVelocity, mPos))
	{
		reset();
	}
}

void Ball::drawGameBall() const
{
	activateBallShader();
	{
		glPushMatrix();
		{
			glTranslatef(mPos.x, mPos.y, mPos.z);
			glScalef(mRadius, mRadius, mRadius);
			quaternionTransform(mOrientation);

			/* explosion? */
			if (mIsBallInPieces)
			{
				drawExplosion();
			}
			else
			{
				drawBall();
			}
		}
		glPopMatrix();
	}
	deactivateBallShader();
}

void Ball::explodeBall()
{
	Vector3 pos = this->mPos;
	Vector3 speed = this->mVelocity;

	reset();

	this->mVelocity.z = -10.0f;

	mExplosion.init(pos, speed, this->mPos, this->mVelocity);

	this->mPos = pos;
	this->mVelocity = speed;

	mIsBallInPieces = true;
}


bool collisionPoint(const Vector3 sphere, const Vector3* quad,
									 const Vector3 normal, float radius, Vector3* collision)
{
	float dToPlane = dot(sphere, normal) - dot(quad[0], normal);
	int inside = 1;

	int i;

	if (fabs(dToPlane) >= radius)
	{
		return false;
	}

	for (i = 0; i < 4; i++)
	{
		int j = (i + 1) % 4;
		Vector3 edge = sub(quad[j], quad[i]);

		if (len(edge) > 0.0f)
		{
			Vector3 n = norm(cross(edge, normal));

			if (dot(sphere, n) - dot(quad[i], n) >= 0.0f)
			{
				Vector3 a = sub(sphere, quad[i]);
				Vector3 nn = norm(cross(cross(edge, a), edge));
				float dToEdge = dot(sphere, nn) - dot(quad[i], nn);
				float f = dot(a, edge) / sqr(len(edge));

				inside = 0;

				if (dToEdge >= radius)
				{
					return false;
				}

				if (f <= 0.0f)
				{
					if (len(sub(sphere, quad[i])) < radius)
					{
						*collision = quad[i];

						return true;
					}
				}
				else if (f >= 1.0f)
				{
					if (len(sub(sphere, quad[j])) < radius)
					{
						*collision = quad[j];

						return true;
					}
				}
				else
				{
					*collision = sub(sphere, scale(dToEdge, nn));

					return true;
				}
			}
		}
	}

	if (inside)
	{
		*collision = sub(sphere, scale(dToPlane, normal));

		return true;
	}

	return false;
}

void Ball::animateBall(float interval)
{
	int collision = 0;
	int q;
	int x;
	int y;
	int dx;
	int dy;

	Vector3 normal(0.0f, 0.0f, 0.0f);

	Vector3 ball;
	Vector3 step;

	mVelocity = add(mVelocity, scale(MOVE_FORCE / mMass * interval, mPushDirection));

	mPushDirection = Vector3(0.0f, 0.0f, 0.0f);

	mVelocity.z -= GRAVITY * interval;

	/* collision detection */

	step = scale(interval, mVelocity);

	ball = add(mPos, step);

	x = (int) floor(ball.x - sgLevel.origin.x);
	y = (int) floor(ball.y - sgLevel.origin.y);

	/* check only fields near by the ball. check field under ball first!!! */
	for (dx = 1; dx <= 3; dx++)
	{
		for (dy = 1; dy <= 3; dy++)
		{
			int start;
			int end;

			getVertIndex(x + (dx % 3) - 1, y + (dy % 3) - 1, &start, &end);

			for (q = start; q < end; q += 4)
			{
				Vector3 *quad = &sgVertices[q];
				Vector3 dir = sgNormals[q];

				Vector3 a;

				if (collisionPoint(ball, quad, dir, mRadius, &a))
				{
#if SHOW_COLLISION_QUADS
					static Color4 red = { 1.0f, 0.0f, 0.0f, 1.0f };
#endif

					/* dist = vector from ball center to quad */
					Vector3 dist = sub(a, ball);
					float l = len(dist);

					/* move = vector to move the ball out of quad */
					Vector3 move = scale(-((mRadius - l) / l), dist);

					/* some rotation for a better look */
					Vector3 right = norm(cross(dir, step));
					Vector3 forward = norm(cross(right, dir));

					mAngularRate =
						scale(dot(sub(ball, mPos), forward) /
									(2.0f * M_PI * mRadius) * 360.0f / interval, right);

					ball = add(ball, move);

					normal = add(normal, move);
					collision = 1;

#if SHOW_COLLISION_QUADS
					setSquareColor(q, red);
#endif
				}
			}
		}
	}

	mPos = ball;

	normal = norm(normal);

	mHasBallHitGoal = false;

	/* contact to surface? */
	if (collision)
	{
		float vn = dot(mVelocity, normal);
		Vector3 rebound = scale(-(1 + ELASTICITY) * vn, normal);

		if (len(rebound) > 3.0f * JUMP_FORCE * interval)
		{
			/* collision was to havy */
			explodeBall();
		}
		else
		{
			mVelocity = add(mVelocity, rebound);

			/* jump */
			if (isKeyPressed(' '))
			{
				mVelocity = add(mVelocity, scale(JUMP_FORCE / mMass * interval, normal));
			}
		}

		if (x == sgLevel.finish.x && y == sgLevel.finish.y)
		{
			/* hit goal */
			mHasBallHitGoal = true;
		}
	}

	/***/

	/* damping */
	mVelocity = scale(DAMPING, mVelocity);

	Quaternion rotation = mkQuaternion(len(mAngularRate) * interval, mAngularRate);
	mOrientation = mulQuaternion(rotation, mOrientation);

	/* falling to infinity */
	if (mPos.z < -1.0f)
	{
		explodeBall();
	}

	/* reset through user */
	if (wasKeyPressed(KEY_ENTER))
	{
		explodeBall();
	}
}

void Ball::drawBall() const
{
	mBallObject.drawDefault();
}

void Ball::drawExplosion() const
{
	mExplosion.drawDefault();
}

void Ball::activateBallShader() const
{
	float lightPos[4] = { 0.0f, 0.0f, 1.0f, 0.0f };
	glEnable(GL_LIGHT0);
	glLightfv(GL_LIGHT0, GL_POSITION, lightPos);

	glEnable(GL_LIGHTING);
	glEnable(GL_COLOR_MATERIAL);

	Vector3 normal(0.0f, 0.0f, 1.0f);
	float light = approximation(mPos, normal);

	if (hasBallTexture())
	{
		glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, sTextureBall);

		ColorStack::colorStack.setColor(Color4(light, light, light));
	}
	else
	{
		ColorStack::colorStack.setColor(Color4(light, 0.0f, 0.0f));
	}
}

void Ball::deactivateBallShader() const
{
	if (hasBallTexture())
	{
		glDisable(GL_TEXTURE_2D);
	}

	glDisable(GL_COLOR_MATERIAL);
	glDisable(GL_LIGHTING);
}
