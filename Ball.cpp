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

#include "Ball.hpp"

#include "level.hpp"
#include "features.hpp"
#include "fbuffer.hpp"
#include "texture.hpp"
#include "lightmap.hpp"
#include "objects.hpp"
#include "main.hpp"
#include "Game.hpp"
#include "keyboard.hpp"
#include "field.hpp"

#include "math/vector.hpp"

#include "functions.hpp"

#include "debug.hpp"

#include <GL/gl.h>

#include <math.h>
#include <string.h>

#define SHOW_COLLISION_QUADS 0

#define CUBE_MAP_SIZE 128

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
	mRadius(BALL_RADIUS),
	mIsReflectionDirty(true),
	mBallLayout(0)
{
	mMass = 1.0f;

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

	mVelocity.x = 0.0f;
	mVelocity.y = 0.0f;
	mVelocity.z = 0.0f;

	mAngularRate.x = 0.0f;
	mAngularRate.y = 0.0f;
	mAngularRate.z = 0.0f;

	mIsBallInPieces = false;
	mIsReflectionDirty = true;
}

void Ball::initCubeMap()
{
	Matrix m;
	int i;

	initProjectMat(m, 90.0f);

	/* init framebuffer for cubemap */
	mCubeMapBall = initFBufferCube(CUBE_MAP_SIZE, CUBE_MAP_SIZE, &mTargetCube[0]);

	for (i = 0; i < 6; i++)
	{
		memcpy(&mViewportCube[i].projection[0][0], &m, sizeof(Matrix));

		mTargetCube[i].viewport = &mViewportCube[i];
	}
}

void Ball::changeBall(int layout)
{
	mBallLayout = layout;
}

void Ball::updateReflection()
{
	static Vector3 lookat[] = {
			Vector3(1.0f, 0.0f, 0.0f),
			Vector3(-1.0f, 0.0f, 0.0f),

			Vector3(0.0f, 1.0f, 0.0f),
			Vector3(0.0f, -1.0f, 0.0f),

			Vector3(0.0f, 0.0f, 1.0f),
			Vector3(0.0f, 0.0f, -1.0f)
	};

	static Vector3 up[] = {
			Vector3(0.0f, -1.0f, 0.0f),
			Vector3(0.0f, -1.0f, 0.0f),

			Vector3(0.0f, 0.0f, 1.0f),
			Vector3(0.0f, 0.0f, -1.0f),

			Vector3(0.0f, -1.0f, 0.0f),
			Vector3(0.0f, -1.0f, 0.0f)
	};

	if (useBallReflection() && mIsReflectionDirty)
	{
		int i;

		glMatrixMode(GL_PROJECTION);
		glLoadMatrixf(&mViewportCube[0].projection[0][0]);

		glViewport(0, 0, CUBE_MAP_SIZE, CUBE_MAP_SIZE);
		TIME(glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, mTargetCube[0].framebuffer));

		for (i = 0; i < 6; i++)
		{
			RenderTarget *target = &mTargetCube[i];
			Viewport *v = target->viewport;

#if DEBUG_TIME
			PRINT_INT(i);
#endif
			TIME(glFramebufferTexture2DEXT
					 (GL_FRAMEBUFFER_EXT, GL_COLOR_ATTACHMENT0_EXT,
						GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, mCubeMapBall, 0));

			glClear(GL_COLOR_BUFFER_BIT);

			glMatrixMode(GL_PROJECTION);
			glLoadMatrixf(&v->projection[0][0]);

			glMatrixMode(GL_MODELVIEW);
			glLoadIdentity();
			gluLookAt(mPos.x, mPos.y, mPos.z,
								mPos.x + lookat[i].x, mPos.y + lookat[i].y, mPos.z + lookat[i].z,
								up[i].x, up[i].y, up[i].z);

			TIME(drawGameBallReflection());
		}

		TIME(glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, 0));

		mIsReflectionDirty = false;
	}
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
	else if (updateExplosion(interval, &mVelocity, &mPos))
	{
		reset();
		resetGameTime();
	}

	mIsReflectionDirty = true;
}

void Ball::drawGameBall() const
{
	bool shader = useBallShader();

	activateBallShader();
	{
		if (mBallLayout == BALL_LAYOUT_DEFAULT)
		{
			float mPos[4] = { 0.0f, 0.0f, 1.0f, 0.0f };

			glEnable(GL_LIGHT0);
			glLightfv(GL_LIGHT0, GL_POSITION, mPos);

			glEnable(GL_LIGHTING);
		}

		glPushMatrix();
		{
			glTranslatef(mPos.x, mPos.y, mPos.z);
			glScalef(mRadius, mRadius, mRadius);
			quaternionTransform(mOrientation);

			/* explosion? */
			if (mIsBallInPieces)
			{
				drawExplosion(shader);
			}
			else
			{
				drawBallObject(shader);
			}
		}
		glPopMatrix();

		if (mBallLayout == BALL_LAYOUT_DEFAULT)
		{
			glDisable(GL_LIGHTING);
		}
	}
	deactivateBallShader();
}

void Ball::explodeBall()
{
	Vector3 pos = this->mPos;
	Vector3 speed = this->mVelocity;

	reset();

	this->mVelocity.z = -10.0f;

	initExplosion(pos, speed, this->mPos, this->mVelocity);

	this->mPos = pos;
	this->mVelocity = speed;

	mIsBallInPieces = true;
	mIsReflectionDirty = true;
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

	mIsReflectionDirty = true;
}

bool Ball::useBallShader(void) const
{
	return hasGolfballShader() && (mBallLayout == BALL_LAYOUT_GOLFBALL
																 || mBallLayout == BALL_LAYOUT_GOLFBALL_METAL);
}

bool Ball::useBallReflection(void) const
{
	return hasFramebuffer() && (mBallLayout == BALL_LAYOUT_METAL
															|| mBallLayout == BALL_LAYOUT_GOLFBALL_METAL);
}

void Ball::activateBallShader() const
{
	Vector3 normal(0.0f, 0.0f, 1.0f);
	float light = approximation(mPos, normal);

	glEnable(GL_COLOR_MATERIAL);
	glColor3f(light, light, light);

	switch (mBallLayout)
	{
	case BALL_LAYOUT_DEFAULT:
		{
			glColor3f(light, 0.0f, 0.0f);
			break;
		}
	case BALL_LAYOUT_TEXTURE:
		{
			glEnable(GL_TEXTURE_2D);
			glBindTexture(GL_TEXTURE_2D, sTextureBall);

			break;
		}
	case BALL_LAYOUT_METAL:
		glTexGeni(GL_S, GL_TEXTURE_GEN_MODE, GL_REFLECTION_MAP);
		glTexGeni(GL_T, GL_TEXTURE_GEN_MODE, GL_REFLECTION_MAP);
		glTexGeni(GL_R, GL_TEXTURE_GEN_MODE, GL_REFLECTION_MAP);

		glEnable(GL_TEXTURE_GEN_S);
		glEnable(GL_TEXTURE_GEN_T);
		glEnable(GL_TEXTURE_GEN_R);

		glColor3f(1.0f, 1.0f, 1.0f);
		break;
	case BALL_LAYOUT_GOLFBALL:
		break;
	case BALL_LAYOUT_GOLFBALL_METAL:
		break;
	}

	if (useBallReflection())
	{
		int x;
		int y;
		Matrix modelview;
		Matrix texture;

		glGetFloatv(GL_MODELVIEW_MATRIX, &modelview[0][0]);

		for (x = 0; x < 4; x++)
		{
			for (y = 0; y < 4; y++)
			{
				if (x < 3 && y < 3)
				{
					texture[x][y] = modelview[y][x];
				}
				else
				{
					texture[x][y] = (x == y);
				}
			}
		}

		glMatrixMode(GL_TEXTURE);
		glPushMatrix();
		glMultMatrixf(&texture[0][0]);

		glMatrixMode(GL_MODELVIEW);

		glEnable(GL_TEXTURE_CUBE_MAP_EXT);
		glBindTexture(GL_TEXTURE_CUBE_MAP_EXT, mCubeMapBall);
	}

	if (useBallShader())
	{
		float reflection;

		if (useBallReflection())
		{
			reflection = 0.7f;
		}
		else
		{
			reflection = 0.0f;

			glEnable(GL_TEXTURE_CUBE_MAP_EXT);
			glBindTexture(GL_TEXTURE_CUBE_MAP_EXT, 0);
		}

		glUseProgram(sgGolfballShader);

		glUniform1i(glGetUniformLocation(sgGolfballShader, "Environment"), 0);
		glUniform1f(glGetUniformLocation(sgGolfballShader, "reflection"),
								reflection);
	}
}

void Ball::deactivateBallShader() const
{
	glDisable(GL_COLOR_MATERIAL);

	switch (mBallLayout)
	{
	case BALL_LAYOUT_DEFAULT:
		break;
	case BALL_LAYOUT_TEXTURE:
		glDisable(GL_TEXTURE_2D);
		break;
	case BALL_LAYOUT_METAL:
		glDisable(GL_TEXTURE_GEN_S);
		glDisable(GL_TEXTURE_GEN_T);
		glDisable(GL_TEXTURE_GEN_R);
		break;
	case BALL_LAYOUT_GOLFBALL:
		break;
	case BALL_LAYOUT_GOLFBALL_METAL:
		break;
	}

	if (useBallReflection())
	{
		glDisable(GL_TEXTURE_CUBE_MAP_EXT);

		glMatrixMode(GL_TEXTURE);
		glPopMatrix();

		glMatrixMode(GL_MODELVIEW);
	}

	if (useBallShader())
	{
		glUseProgram(0);

		if (!useBallReflection())
		{
			glDisable(GL_TEXTURE_CUBE_MAP_EXT);
		}
	}
}

