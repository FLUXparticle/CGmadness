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
#include "math/closestpoint.hpp"

#include "functions.hpp"

#include "macros.hpp"

#include GL_H

#include <math.h>
#include <string.h>

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

static float getMaxZValue(const Square* square)
{
	float res = square->vertices[0].z;
	for (int i = 1; i < 4; i++)
	{
		if (square->vertices[i].z > res)
		{
			res = square->vertices[i].z;
		}
	}
	return res;
}

void Ball::reset()
{
	const Square& roofSquare = getRoofSquare(sgLevel.start.x, sgLevel.start.y);

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

void Ball::animateBall(float interval)
{
	bool collision = false;

	Vector3 normal(0.0f, 0.0f, 0.0f);

	Vector3 ball;
	Vector3 step;

	mVelocity = add(mVelocity, scale(MOVE_FORCE / mMass * interval, mPushDirection));

	mPushDirection = Vector3(0.0f, 0.0f, 0.0f);

	mVelocity.z -= GRAVITY * interval;

	/* collision detection */

	step = scale(interval, mVelocity);

	ball = mPos + step;

	/* check only fields near by the ball. check field under ball first!!! */
	QuadList list = getFieldSphereIntersection(ball, BALL_RADIUS);

	while (list.next())
	{
		Quad quad = *list;
		const Vector3* q = quad.mVertices;
		const Vector3& dir = quad.mNormals[0];

		Vector3 a1 = closestPointTriangle(ball, q[0], q[1], q[2]);
		Vector3 a2 = closestPointTriangle(ball, q[2], q[3], q[0]);

		float d1 = (ball - a1).len();
		float d2 = (ball - a2).len();

		Vector3 a = (d1 <= d2) ? a1 : a2;

		if ((a - ball).len() <= BALL_RADIUS)
		{
			/* dist = vector from ball center to quad */
			Vector3 dist = a - ball;
			float l = dist.len();

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
			collision = true;
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

		int x = (int) floor(ball.x - sgLevel.origin.x);
		int y = (int) floor(ball.y - sgLevel.origin.y);

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
	glLightfv(GL_LIGHT0, GL_POSITION, lightPos);

	glEnable(GL_LIGHTING);

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

	glDisable(GL_LIGHTING);
}
