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

#include <GL/glew.h>

#include "graphics/objects.hpp"
#include "hw/features.hpp"
#include "hw/fbuffer.hpp"
#include "process/Game.hpp"
#include "lightmap.hpp"
#include "process/Main.hpp"

#include "ColorStack.hpp"

#include "math/Matrix.hpp"

#include "debug.hpp"

#include <GL/glew.h>
#include GLU_H

#include <cstring>

#define CUBE_MAP_SIZE 128

PlayersBall PlayersBall::sgoBall;

std::vector<int> PlayersBall::gBallLayouts;

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

	gBallLayouts.push_back(BALL_LAYOUT_DEFAULT);

	if (Ball::hasBallTexture())
	{
		gBallLayouts.push_back(BALL_LAYOUT_TEXTURE);
	}

	if (sgoBall.hasCubeMap())
	{
		gBallLayouts.push_back(BALL_LAYOUT_METAL);
	}

	if (hasGolfballShader())
	{
		gBallLayouts.push_back(BALL_LAYOUT_GOLFBALL);
	}

	if (hasGolfballShader() && PlayersBall::sgoBall.hasCubeMap())
	{
		gBallLayouts.push_back(BALL_LAYOUT_GOLFBALL_METAL);
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
		mBallObject.drawShader();
	}
	else
	{
		mBallObject.drawDefault();
	}
}

void PlayersBall::drawExplosion() const
{
	if (useBallShader())
	{
		mExplosion.drawShader();
	}
	else
	{
		mExplosion.drawDefault();
	}
}

void PlayersBall::activateBallShader() const
{
	Vector3 normal(0.0f, 0.0f, 1.0f);
	float light = approximation(mPos, normal);

	ColorStack::colorStack.setColor(Color4(light, light, light));

	switch (ballLayout())
	{
	case BALL_LAYOUT_DEFAULT:
		{
			float mPos[4] = { 0.0f, 0.0f, 1.0f, 0.0f };
			glLightfv(GL_LIGHT0, GL_POSITION, mPos);

			glEnable(GL_LIGHTING);

			ColorStack::colorStack.setColor(Color4(light, 0.0f, 0.0f));
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

		sgGolfballShader->useProgram();

		sgGolfballShader->setVariable("Environment", 0);
		sgGolfballShader->setVariable("reflection", reflection);
	}
}

void PlayersBall::deactivateBallShader() const
{
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
