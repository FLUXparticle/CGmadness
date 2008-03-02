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

#include "PlayersBall.hpp"

#include "objects.hpp"
#include "features.hpp"
#include "fbuffer.hpp"
#include "Game.hpp"
#include "lightmap.hpp"
#include "main.hpp"

#include "math/vector.hpp"

#include "debug.hpp"

#include <string.h>

#define CUBE_MAP_SIZE 128

PlayersBall PlayersBall::sgoBall;

PlayersBall::PlayersBall() :
	mIsReflectionDirty(true),
	mBallLayout(0)
{
  // empty
}

PlayersBall::~PlayersBall()
{
  // empty
}

void PlayersBall::init()
{
	if (hasFramebuffer())
	{
		sgoBall.initCubeMap();
	}

	sgoBall.gBallLayouts.push_back(BALL_LAYOUT_DEFAULT);

	if (Ball::hasBallTexture())
	{
		sgoBall.gBallLayouts.push_back(BALL_LAYOUT_TEXTURE);
	}

	if (sgoBall.hasCubeMap())
	{
		sgoBall.gBallLayouts.push_back(BALL_LAYOUT_METAL);
	}

	if (hasGolfballShader())
	{
		sgoBall.gBallLayouts.push_back(BALL_LAYOUT_GOLFBALL);
	}

	if (hasGolfballShader() && PlayersBall::sgoBall.hasCubeMap())
	{
		sgoBall.gBallLayouts.push_back(BALL_LAYOUT_GOLFBALL_METAL);
	}
}

void PlayersBall::initCubeMap()
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

bool PlayersBall::hasCubeMap() const
{
	return mCubeMapBall != 0;
}

void PlayersBall::reset()
{
	Ball::reset();
	mIsReflectionDirty = true;
}

void PlayersBall::update(float interval)
{
	Ball::update(interval);
	mIsReflectionDirty = true;
}

void PlayersBall::updateReflection(const BallReflection* reflection)
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

			TIME(reflection->drawBallReflection());
		}

		TIME(glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, 0));

		mIsReflectionDirty = false;
	}
}

void PlayersBall::drawMenuBall() const
{
	glPushMatrix();
	{
		glRotatef(-90.0f, 1.0f, 0.0f, 0.0f);

		activateBallShader();
		{
			drawBall();
		}
		deactivateBallShader();
	}
	glPopMatrix();
}

void PlayersBall::drawBall() const
{
	if (useBallShader())
	{
		drawBallObjectShader();
	}
	else
	{
		drawBallObjectDefault();
	}
}

void PlayersBall::drawExplosion() const
{
	if (useBallShader())
	{
		drawExplosionShader();
	}
	else
	{
		drawExplosionDefault();
	}
}

void PlayersBall::activateBallShader() const
{
	Vector3 normal(0.0f, 0.0f, 1.0f);
	float light = approximation(mPos, normal);

	glEnable(GL_COLOR_MATERIAL);
	glColor3f(light, light, light);

	switch (ballLayout())
	{
	case BALL_LAYOUT_DEFAULT:
		{
			float mPos[4] = { 0.0f, 0.0f, 1.0f, 0.0f };

			glEnable(GL_LIGHT0);
			glLightfv(GL_LIGHT0, GL_POSITION, mPos);

			glEnable(GL_LIGHTING);

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

void PlayersBall::deactivateBallShader() const
{
	glDisable(GL_COLOR_MATERIAL);

	switch (ballLayout())
	{
	case BALL_LAYOUT_DEFAULT:
		glDisable(GL_LIGHTING);
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

bool PlayersBall::useBallShader(void) const
{
	return hasGolfballShader() && (ballLayout() == BALL_LAYOUT_GOLFBALL
																 || ballLayout() == BALL_LAYOUT_GOLFBALL_METAL);
}

bool PlayersBall::useBallReflection(void) const
{
	return hasFramebuffer() && (ballLayout() == BALL_LAYOUT_METAL
															|| ballLayout() == BALL_LAYOUT_GOLFBALL_METAL);
}

int PlayersBall::ballLayout() const
{
	return gBallLayouts[mBallLayout];
}
