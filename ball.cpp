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

#include "ball.hpp"

#include "level.hpp"
#include "Game.hpp"
#include "callback.hpp"
#include "objects.hpp"
#include "shader.hpp"
#include "fbuffer.hpp"
#include "vector.hpp"
#include "features.hpp"
#include "keyboard.hpp"
#include "texture.hpp"
#include "field.hpp"
#include "lightmap.hpp"
#include "camera.hpp"

#include "functions.hpp"

#include "debug.hpp"

#include <GL/glew.h>
#include <GL/glut.h>

#include <stdio.h>
#include <string.h>
#include <math.h>

#include <assert.h>

#define MOVE_FORCE 5.0f
#define GRAVITY 9.81f
#define JUMP_FORCE (50.0f * MOVE_FORCE)
#define DAMPING 0.99f
#define ELASTICITY 0.5f

#define CUBE_MAP_SIZE 128

#define SHOW_COLLISION_QUADS 0

Ball sgoBall;
bool sgIsBallInPieces;
bool sgHasBallHitGoal;
bool sgIsMouseControl = false;

Vector3 sgForward;
Vector3 sgRight;

static bool gIsBallCameraActive;

static float gDistance;
static float gLatitude;
static float gLongitude;

static int gDragX = 0;
static int gDragY = 0;

static RenderTarget gTargetCube[6];
static Viewport gViewportCube[6];

static int gBallLayout = 0;

static bool gIsReflectionDirty = true;

static int gCubeMapBall;

static int gTextureBall;

bool hasBallTexture(void)
{
	return gTextureBall >= 0;
}

bool hasCubeMap(void)
{
	return gCubeMapBall != 0;
}

bool useBallShader(void)
{
	return hasGolfballShader() && (gBallLayout == BALL_LAYOUT_GOLFBALL
																 || gBallLayout == BALL_LAYOUT_GOLFBALL_METAL);
}

bool useBallReflection(void)
{
	return hasFramebuffer() && (gBallLayout == BALL_LAYOUT_METAL
															|| gBallLayout == BALL_LAYOUT_GOLFBALL_METAL);
}

void changeBall(int layout)
{
	gBallLayout = layout;
}

void gameDrag(int dx, int dy)
{
	gDragX += dx;
	gDragY += dy;
}

void updateReflection(void)
{
	static Vector3 lookat[] = {
		{1.0f, 0.0f, 0.0f},
		{-1.0f, 0.0f, 0.0f},

		{0.0f, 1.0f, 0.0f},
		{0.0f, -1.0f, 0.0f},

		{0.0f, 0.0f, 1.0f},
		{0.0f, 0.0f, -1.0f}
	};

	static Vector3 up[] = {
		{0.0f, -1.0f, 0.0f},
		{0.0f, -1.0f, 0.0f},

		{0.0f, 0.0f, 1.0f},
		{0.0f, 0.0f, -1.0f},

		{0.0f, -1.0f, 0.0f},
		{0.0f, -1.0f, 0.0f}
	};

	if (useBallReflection() && gIsReflectionDirty)
	{
		int i;

		glMatrixMode(GL_PROJECTION);
		glLoadMatrixf(&gViewportCube[0].projection[0][0]);

		glViewport(0, 0, CUBE_MAP_SIZE, CUBE_MAP_SIZE);
		TIME(glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, gTargetCube[0].framebuffer));

		for (i = 0; i < 6; i++)
		{
			RenderTarget *target = &gTargetCube[i];
			Viewport *v = target->viewport;

#if DEBUG_TIME
			PRINT_INT(i);
#endif
			TIME(glFramebufferTexture2DEXT
					 (GL_FRAMEBUFFER_EXT, GL_COLOR_ATTACHMENT0_EXT,
						GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, gCubeMapBall, 0));

			glClear(GL_COLOR_BUFFER_BIT);

			glMatrixMode(GL_PROJECTION);
			glLoadMatrixf(&v->projection[0][0]);

			glMatrixMode(GL_MODELVIEW);
			glLoadIdentity();
			gluLookAt(sgoBall.pos.x, sgoBall.pos.y, sgoBall.pos.z,
								sgoBall.pos.x + lookat[i].x, sgoBall.pos.y + lookat[i].y,
								sgoBall.pos.z + lookat[i].z, up[i].x, up[i].y, up[i].z);

			TIME(drawGameBallReflection());
		}

		TIME(glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, 0));

		gIsReflectionDirty = false;
	}
}

void resetBall(void)
{
	Square roofSquare;
	getRoofSquare(sgLevel.start.x, sgLevel.start.y, &roofSquare);
	sgoBall.pos.x = roofSquare.mid.x;
	sgoBall.pos.y = roofSquare.mid.y;
	sgoBall.pos.z = getMaxZValue(&roofSquare) + 2.5f;

	sgoBall.velocity.x = 0.0f;
	sgoBall.velocity.y = 0.0f;
	sgoBall.velocity.z = 0.0f;

	sgoBall.angularRate.x = 0.0f;
	sgoBall.angularRate.y = 0.0f;
	sgoBall.angularRate.z = 0.0f;

	sgIsBallInPieces = false;
	gIsReflectionDirty = true;
}

void explodeBall(void)
{
	Vector3 pos = sgoBall.pos;
	Vector3 speed = sgoBall.velocity;

	resetBall();

	sgoBall.velocity.z = -10.0f;

	initExplosion(pos, speed, sgoBall.pos, sgoBall.velocity);

	sgoBall.pos = pos;
	sgoBall.velocity = speed;

	sgIsBallInPieces = true;
	gIsReflectionDirty = true;
}

void initCubeMap(void)
{
	Matrix m;
	int i;

	initProjectMat(m, 90.0f);

	/* init framebuffer for cubemap */
	gCubeMapBall = initFBufferCube(CUBE_MAP_SIZE, CUBE_MAP_SIZE, &gTargetCube[0]);

	for (i = 0; i < 6; i++)
	{
		memcpy(&gViewportCube[i].projection[0][0], &m, sizeof(Matrix));

		gTargetCube[i].viewport = &gViewportCube[i];
	}
}

void initBall(void)
{
	if (hasFramebuffer())
	{
		initCubeMap();
	}

	gTextureBall = loadTexture("data/ball.tga", false);

	sgoBall.mass = 1.0f;
	sgoBall.radius = 0.2f;

	sgoBall.orientation = mkQuaternion(0.0f, vector3(0.0f, 0.0f, 1.0f));

	setPreDisplayFunc(updateReflection);
}

bool collisionPoint(const Vector3 sphere, const Vector3 * quad,
									 const Vector3 normal, Vector3 * collision)
{
	float dToPlane = dot(sphere, normal) - dot(quad[0], normal);
	int inside = 1;

	int i;

	if (fabs(dToPlane) >= sgoBall.radius)
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

				if (dToEdge >= sgoBall.radius)
				{
					return false;
				}

				if (f <= 0.0f)
				{
					if (len(sub(sphere, quad[i])) < sgoBall.radius)
					{
						*collision = quad[i];

						return true;
					}
				}
				else if (f >= 1.0f)
				{
					if (len(sub(sphere, quad[j])) < sgoBall.radius)
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

void animateBall(float interval)
{
	int collision = 0;
	int q;
	int x;
	int y;
	int dx;
	int dy;

	Vector3 force = { 0.0f, 0.0f, 0.0f };
	Vector3 normal = { 0.0f, 0.0f, 0.0f };

	Vector3 ball;
	Vector3 step;

	/* ball controls */
	if (sgIsMouseControl)
	{
		if (isKeyPressed('a'))
		{
			force = sub(force, sgRight);
		}

		if (isKeyPressed('d'))
		{
			force = add(force, sgRight);
		}

		if (isKeyPressed('s'))
		{
			force = sub(force, sgForward);
		}

		if (isKeyPressed('w'))
		{
			force = add(force, sgForward);
		}
	}
	else
	{
		if (isCursorPressed(CURSOR_LEFT))
		{
			force = sub(force, sgRight);
		}

		if (isCursorPressed(CURSOR_RIGHT))
		{
			force = add(force, sgRight);
		}

		if (isCursorPressed(CURSOR_DOWN))
		{
			force = sub(force, sgForward);
		}

		if (isCursorPressed(CURSOR_UP))
		{
			force = add(force, sgForward);
		}
	}

	force = norm(force);

	sgoBall.velocity =
		add(sgoBall.velocity, scale(MOVE_FORCE / sgoBall.mass * interval, force));

	sgoBall.velocity.z -= GRAVITY * interval;

	/* collision detection */

	step = scale(interval, sgoBall.velocity);

	ball = add(sgoBall.pos, step);

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

				if (collisionPoint(ball, quad, dir, &a))
				{
#if SHOW_COLLISION_QUADS
					static Color4 red = { 1.0f, 0.0f, 0.0f, 1.0f };
#endif

					/* dist = vector from ball center to quad */
					Vector3 dist = sub(a, ball);
					float l = len(dist);

					/* move = vector to move the ball out of quad */
					Vector3 move = scale(-((sgoBall.radius - l) / l), dist);

					/* some rotation for a better look */
					Vector3 right = norm(cross(dir, step));
					Vector3 forward = norm(cross(right, dir));

					sgoBall.angularRate =
						scale(dot(sub(ball, sgoBall.pos), forward) /
									(2.0f * PI * sgoBall.radius) * 360.0f / interval, right);

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

	sgoBall.pos = ball;

	normal = norm(normal);

	sgHasBallHitGoal = false;

	/* contact to surface? */
	if (collision)
	{
		float vn = dot(sgoBall.velocity, normal);
		Vector3 rebound = scale(-(1 + ELASTICITY) * vn, normal);

		if (len(rebound) > 3.0f * JUMP_FORCE * interval)
		{
			/* collision was to havy */
			explodeBall();
		}
		else
		{
			sgoBall.velocity = add(sgoBall.velocity, rebound);

			/* jump */
			if (isKeyPressed(' '))
			{
				sgoBall.velocity =
					add(sgoBall.velocity,
							scale(JUMP_FORCE / sgoBall.mass * interval, normal));
			}
		}

		if (x == sgLevel.finish.x && y == sgLevel.finish.y)
		{
			/* hit goal */
			sgHasBallHitGoal = true;
		}
	}

	/***/

	/* damping */
	sgoBall.velocity = scale(DAMPING, sgoBall.velocity);

	sgoBall.orientation =
		mulQuaternion(mkQuaternion
									(len(sgoBall.angularRate) * interval, sgoBall.angularRate),
									sgoBall.orientation);

	/* falling to infinity */
	if (sgoBall.pos.z < -1.0f)
	{
		explodeBall();
	}

	/* reset through user */
	if (wasKeyPressed(KEY_ENTER))
	{
		explodeBall();
	}

	gIsReflectionDirty = true;
}

void resetBallCamera(void)
{
	gDistance = 5.0f;
	gLatitude = 20.0f;
	gLongitude = 0.0f;
}

void enableBallCamera(void)
{
	if (sgIsMouseControl)
	{
		setDragFunc(gameDrag);
	}

	glutSetCursor(GLUT_CURSOR_NONE);

	gIsBallCameraActive = true;
}

void disableBallCamera(void)
{
	setDragFunc(NULL);

	gIsBallCameraActive = false;
}

void updateBallCamera(float interval, Vector3 ball)
{
	Vector3 diff;
	Vector3 up = { 0.0f, 0.0f, 1.0f };
	static Vector3 dest = { 0.0f, 0.0f, 0.0f };

	/* game controls for camera */

	if (sgIsMouseControl)
	{
		gLongitude -= 5.0f * interval * gDragX;
		gLatitude += 5.0f * interval * gDragY;

		gDragX = 0;
		gDragY = 0;
	}
	else
	{
		/* zoom */
		if (isKeyPressed('f') && gDistance < 20.0f)
			gDistance += 0.1f;
		if (isKeyPressed('r') && gDistance > 0.5)
			gDistance -= 0.1f;

		/* rotation */
		if (isKeyPressed('a'))
			gLongitude -= 120.0f * interval;
		if (isKeyPressed('d'))
			gLongitude += 120.0f * interval;

		/* height */
		if (isKeyPressed('w'))
			gLatitude += 120.0f * interval;
		if (isKeyPressed('s'))
			gLatitude -= 120.0f * interval;

		gLatitude = clamp(gLatitude, -89.0f, 89.0f);
	}

	dest.x =
		ball.x +
		gDistance * sin(gLongitude * PI / 180.0f) * cos(gLatitude * PI / 180.0f);
	dest.y =
		ball.y -
		gDistance * cos(gLongitude * PI / 180.0f) * cos(gLatitude * PI / 180.0f);
	dest.z = ball.z + gDistance * sin(gLatitude * PI / 180.0f);

	moveCamera(interval, dest, ball);

	if (sgIsMouseControl)
	{
		sgCamera = dest;
	}

	diff = sub(sgLookat, sgCamera);
	diff.z = 0.0f;
	sgForward = norm(diff);
	sgRight = norm(cross(sgForward, up));
}

void updateBall(float interval)
{
	if (!sgIsBallInPieces)
	{
		animateBall(interval);
	}
	else if (updateExplosion(interval, &sgoBall.velocity, &sgoBall.pos))
	{
		resetBall();
		resetGameTime();
	}

	if (gIsBallCameraActive)
	{
		updateBallCamera(interval, sgoBall.pos);
	}

	gIsReflectionDirty = true;
}

void activateBallShader(void)
{
	Vector3 normal = vector3(0.0f, 0.0f, 1.0f);
	float light = approximation(sgoBall.pos, normal);

	glEnable(GL_COLOR_MATERIAL);
	glColor3f(light, light, light);

	switch (gBallLayout)
	{
	case BALL_LAYOUT_DEFAULT:
		{
			glColor3f(light, 0.0f, 0.0f);
			break;
		}
	case BALL_LAYOUT_TEXTURE:
		{
			glEnable(GL_TEXTURE_2D);
			glBindTexture(GL_TEXTURE_2D, gTextureBall);

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
		glBindTexture(GL_TEXTURE_CUBE_MAP_EXT, gCubeMapBall);
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

void deactivateBallShader(void)
{
	glDisable(GL_COLOR_MATERIAL);

	switch (gBallLayout)
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

void drawMenuBall(void)
{
	glPushMatrix();

	glRotatef(-90.0f, 1.0f, 0.0f, 0.0f);

	activateBallShader();

	drawBallObject(useBallShader());

	deactivateBallShader();

	glPopMatrix();
}

void drawGameBall(void)
{
	bool shader = useBallShader();

	activateBallShader();

	if (gBallLayout == BALL_LAYOUT_DEFAULT)
	{
		float pos[4] = { 0.0f, 0.0f, 1.0f, 0.0f };

		glEnable(GL_LIGHT0);
		glLightfv(GL_LIGHT0, GL_POSITION, pos);

		glEnable(GL_LIGHTING);
	}

	glPushMatrix();

	glTranslatef(sgoBall.pos.x, sgoBall.pos.y, sgoBall.pos.z);
	glScalef(sgoBall.radius, sgoBall.radius, sgoBall.radius);
	quaternionTransform(sgoBall.orientation);

	/* explosion? */
	if (sgIsBallInPieces)
	{
		drawExplosion(shader);
	}
	else
	{
		drawBallObject(shader);
	}

	glPopMatrix();

	if (gBallLayout == BALL_LAYOUT_DEFAULT)
	{
		glDisable(GL_LIGHTING);
	}

	deactivateBallShader();
}
