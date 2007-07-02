/*
 * CG Madness - a Marble Madness clone
 * Copyright (C) 2007  Sven Reinck
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
 *
 * $Id$
 *
 */

#include "ball.h"

#include "common.h"
#include "game.h"
#include "callback.h"
#include "objects.h"
#include "shader.h"
#include "fbuffer.h"
#include "vector.h"
#include "features.h"
#include "keyboard.h"
#include "texture.h"
#include "field.h"

#include "debug.h"

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

RenderTarget gTargetCube[6];
static Viewport gViewportCube[6];

static int gIsBallInPieces;

static int gBallLayout = 0;

static int gDirtyReflection = 1;

Ball sgoBall;

static GLhandleARB gShaderBall;
static int gCubeMapBall;

static int gTextureBall;

int hasBallTexture(void) {
	return gTextureBall >= 0;
}

int hasBallReflection(void) {
	return gCubeMapBall != 0;
}

int hasBallShader(void) {
	return gShaderBall != 0;
}

int useBallShader(void) {
	return hasShader() && (gBallLayout == BALL_LAYOUT_GOLFBALL || gBallLayout == BALL_LAYOUT_GOLFBALL_METAL);
}

int useBallReflection(void) {
	return hasFramebuffer() && (gBallLayout == BALL_LAYOUT_METAL || gBallLayout == BALL_LAYOUT_GOLFBALL_METAL);
}

void changeBall(int layout) {
	int i;
	int reflection;

  gBallLayout = layout;

	reflection = useBallReflection();

	for (i = 0; i < 6; i++) {
		gTargetCube[i].enabled = reflection;
	}
}

void updateReflection(void)
{
	static Vector3 lookat[] = {
		{  1.0f,  0.0f,  0.0f },
		{ -1.0f,  0.0f,  0.0f },

		{  0.0f,  1.0f,  0.0f },
		{  0.0f, -1.0f,  0.0f },

		{  0.0f,  0.0f,  1.0f },
		{  0.0f,  0.0f, -1.0f }
	};

	static Vector3 up[] = {
		{ 0.0f, -1.0f,  0.0f },
		{ 0.0f, -1.0f,  0.0f },

		{ 0.0f,  0.0f,  1.0f },
		{ 0.0f,  0.0f, -1.0f },

		{ 0.0f, -1.0f,  0.0f },
    { 0.0f, -1.0f,  0.0f }
	};

	if (useBallReflection() && gDirtyReflection)
	{
		int i;

		glMatrixMode(GL_PROJECTION);
		glLoadMatrixf(&gViewportCube[0].projection[0][0]);

		glViewport(0, 0, CUBE_MAP_SIZE, CUBE_MAP_SIZE);
		TIME(glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, gTargetCube[0].framebuffer));

		for (i = 0; i < 6; i++) {
			RenderTarget* target = &gTargetCube[i];
			Viewport* v = target->viewport;

#if DEBUG_TIME
			PRINT_INT(i);
#endif
			TIME(glFramebufferTexture2DEXT(GL_FRAMEBUFFER_EXT, GL_COLOR_ATTACHMENT0_EXT, GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, gCubeMapBall, 0));

			glClear(GL_COLOR_BUFFER_BIT);

			glMatrixMode(GL_PROJECTION);
			glLoadMatrixf(&v->projection[0][0]);

			glMatrixMode(GL_MODELVIEW);
			glLoadIdentity();
			gluLookAt(
					sgoBall.pos.x, sgoBall.pos.y, sgoBall.pos.z,
					sgoBall.pos.x + lookat[i].x, sgoBall.pos.y + lookat[i].y, sgoBall.pos.z + lookat[i].z,
					up[i].x, up[i].y, up[i].z
					);

 			TIME(drawGameReflection());
		}

		TIME(glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, 0));

		gDirtyReflection = 0;
	}
}

void resetBall(void) {
	Square roofSquare;
	getRoofSquare(sgLevel.start.x, sgLevel.start.y, &roofSquare);
	sgoBall.pos.x = sgLevel.start.x + 0.5f;
	sgoBall.pos.y = sgLevel.start.y + 0.5f;
	sgoBall.pos.z = getMaxZValue(&roofSquare) + 2.5f;

	sgoBall.velocity.x = 0.0f;
	sgoBall.velocity.y = 0.0f;
	sgoBall.velocity.z = 0.0f;

	sgoBall.angularRate.x = 0.0f;
	sgoBall.angularRate.y = 0.0f;
	sgoBall.angularRate.z = 0.0f;

	gIsBallInPieces = 0;
	gDirtyReflection = 1;
}

void explodeBall(void) {
	Vector3 pos = sgoBall.pos;
	Vector3 speed = sgoBall.velocity;

	resetBall();

	sgoBall.velocity.z = -10.0f;

	initExplosion(pos, speed, sgoBall.pos, sgoBall.velocity);

	sgoBall.pos = pos;
	sgoBall.velocity = speed;

	gIsBallInPieces = 1;
	gDirtyReflection = 1;
}

void initCubeMap(void) {
	Matrix m;
	int i;

	initProjectMat(m, 90.0f);

	/* init framebuffer for cubemap */
	gCubeMapBall = initFBufferCube(CUBE_MAP_SIZE, CUBE_MAP_SIZE, &gTargetCube[0]);

	for (i = 0; i < 6; i++) {
		memcpy(&gViewportCube[i].projection[0][0], &m, sizeof(Matrix));
		gViewportCube[i].draw = drawGameReflection;

		gTargetCube[i].viewport = &gViewportCube[i];
	}
}

void initBall(void) {
	if (hasShader()) {
		gShaderBall = makeShader("golfball.vert", "golfball.frag");

		if (gShaderBall == 0) {
			printf("Golfball-Shader not ready :-(\n");
		} else {
			printf("Golfball-Shader ready :-)\n");
		}
	}

	if (hasFramebuffer()) {
		initCubeMap();
	}

	gTextureBall = loadTexture("data/ball.tga", 0);

	sgoBall.mass = 1.0f;
	sgoBall.radius = 0.2f;

	sgoBall.orientation = mkQuaternion(0.0f, vector3(0.0f, 0.0f, 1.0f));

	setPreDisplayFunc(updateReflection);
}

Vector3 collisionPoint(const Vector3 sphere, const Vector3* quad, const Vector3 normal) {
	Vector3 a = midpoint(quad);

	float dToPlane = dot(sphere, normal) - dot(a, normal);
	Vector3 b = sub(sphere, scale(dToPlane, normal));

	int i;

	for (i = 0; i < 4; i++) {
		int j = (i + 1) % 4;
		Vector3 edge = sub(quad[j], quad[i]);

		if (len(edge) > 0.0f) {
			Vector3 e = quad[i];
			Vector3 n = cross(edge, normal);

			float da = dot(e, n) - dot(a, n);
			float db = dot(b, n) - dot(e, n);

			assert(da > 0.0f);

			if (db > 0.0f) {
				float d = da + db;
				b = add(scale(da / d, b), scale(db / d, a));
			}
		}
	}

	return b;
}

void animateBall(float interval) {
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
#if (MOUSE_CONTROL)
	if (isKeyPressed('a')) {
		force = sub(force, sgRight);
	}

	if (isKeyPressed('d')) {
		force = add(force, sgRight);
	}

	if (isKeyPressed('s')) {
		force = sub(force, sgForward);
	}

	if (isKeyPressed('w')) {
		force = add(force, sgForward);
	}
#else
	if (isCursorPressed(CURSOR_LEFT)) {
		force = sub(force, sgRight);
	}

	if (isCursorPressed(CURSOR_RIGHT)) {
		force = add(force, sgRight);
	}

	if (isCursorPressed(CURSOR_DOWN)) {
		force = sub(force, sgForward);
	}

	if (isCursorPressed(CURSOR_UP)) {
		force = add(force, sgForward);
	}
#endif

	force = norm(force);

	sgoBall.velocity = add(sgoBall.velocity, scale(MOVE_FORCE / sgoBall.mass * interval, force));

	sgoBall.velocity.z -= GRAVITY * interval;

	/* collision detection */

	step = scale(interval, sgoBall.velocity);

	ball = add(sgoBall.pos, step);

	x = floor(ball.x);
	y = floor(ball.y);

  /* check only fields near by the ball */
  for (dx = -1; dx <= 1; dx++) {
		for (dy = -1; dy <= 1; dy++) {
			int start;
			int end;

			getVertIndex(x + dx, y + dy, &start, &end);

			for (q = start; q < end; q += 4) {
				Vector3* quad = &sgVertices[q];
				Vector3 dir = sgNormals[q];

				Vector3 a = collisionPoint(ball, quad, dir);

				/* dist = vector from ball center to quad */
				Vector3 dist = sub(a, ball);
				float l = len(dist);

        /* collision? */
				if (l < sgoBall.radius) {
#if SHOW_COLLISION_QUADS
					static Color4 red = { 1.0f, 0.0f, 0.0f, 1.0f };
#endif

					/* move = vector to move the ball out of quad */
					Vector3 move = scale(-((sgoBall.radius - l) / l), dist);

					/* some rotation for a better look */
					Vector3 right = norm(cross(dir, step));
					Vector3 forward = norm(cross(right, dir));

					sgoBall.angularRate = scale(dot(sub(ball, sgoBall.pos), forward) / (2.0f * PI * sgoBall.radius) * 360.0f / interval, right);

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

	/* contact to surface? */
	if (collision) {
		float vn = dot(sgoBall.velocity, normal);
		Vector3 rebound = scale(-(1 + ELASTICITY) * vn, normal);

    if (len(rebound) > 3.0f * JUMP_FORCE * interval) {
			/* collision was to havy */
			explodeBall();
		} else if (floor(sgoBall.pos.x) == sgLevel.finish.x && floor(sgoBall.pos.y) == sgLevel.finish.y) {
			/* reached finish quad */
			loadNewLevel();
		} else {
			sgoBall.velocity = add(sgoBall.velocity, rebound);

			/* jump */
			if (isKeyPressed(' ')) {
				sgoBall.velocity = add(sgoBall.velocity, scale(JUMP_FORCE / sgoBall.mass * interval, normal));
			}
		}
	}

	/***/

	/* damping */
	sgoBall.velocity = scale(DAMPING, sgoBall.velocity);

	sgoBall.orientation = mulQuaternion(mkQuaternion(len(sgoBall.angularRate) * interval, sgoBall.angularRate), sgoBall.orientation);

	/* falling to infinity */
	if (sgoBall.pos.z < -10.0f) {
		explodeBall();
	}

  /* reset through user */
	if (isKeyPressed(KEY_ENTER)) {
		explodeBall();
	}

	gDirtyReflection = 1;
}

void updateBall(float interval) {
	if (!gIsBallInPieces) {
		animateBall(interval);
	} else if (updateExplosion(interval, &sgoBall.velocity, &sgoBall.pos)) {
		resetBall();
	}

	gDirtyReflection = 1;
}

void activateBallShader(void) {
	Vector3 normal = vector3(0.0f, 0.0f, 1.0f);
	float light =	approximation(sgoBall.pos, normal);

	glEnable(GL_COLOR_MATERIAL);
	glColor3f(light, light, light);

	switch (gBallLayout) {
		case BALL_LAYOUT_DEFAULT: {
			glColor3f(light, 0.0f, 0.0f);
			break;
		}
		case BALL_LAYOUT_TEXTURE: {
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

	if (useBallReflection()) {
		int x;
		int y;
		Matrix m;

		for (x = 0; x < 4; x++) {
			for (y = 0; y < 4; y++) {
				if (x < 3 && y < 3) {
					m[x][y] = sgWindowViewport.view[y][x];
				} else {
					m[x][y] = (x == y);
				}
			}
		}

		glMatrixMode(GL_TEXTURE);
		glPushMatrix();
		glMultMatrixf(&m[0][0]);

		glMatrixMode(GL_MODELVIEW);

		glEnable(GL_TEXTURE_CUBE_MAP_EXT);
		glBindTexture(GL_TEXTURE_CUBE_MAP_EXT, gCubeMapBall);
	}

	if (useBallShader()) {
		float reflection;

		if (useBallReflection()) {
			reflection = 0.7f;
		} else {
			reflection = 0.0f;

			glEnable(GL_TEXTURE_CUBE_MAP_EXT);
			glBindTexture(GL_TEXTURE_CUBE_MAP_EXT, 0);
		}

		glUseProgram(gShaderBall);

		glUniform1i(glGetUniformLocation(gShaderBall, "Environment"), 0);
		glUniform1f(glGetUniformLocation(gShaderBall, "reflection"), reflection);
	}
}

void deactivateBallShader(void) {
	glDisable(GL_COLOR_MATERIAL);

	switch (gBallLayout) {
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

	if (useBallReflection()) {
		glDisable(GL_TEXTURE_CUBE_MAP_EXT);

		glMatrixMode(GL_TEXTURE);
		glPopMatrix();

		glMatrixMode(GL_MODELVIEW);
	}

	if (useBallShader()) {
		glUseProgram(0);

		if (!useBallReflection()) {
			glDisable(GL_TEXTURE_CUBE_MAP_EXT);
		}
	}
}

void drawMenuBall(void) {
	activateBallShader();

		drawBallObject(useBallShader());

	deactivateBallShader();
}

void drawGameBall(void) {
	int shader = useBallShader();

	activateBallShader();

		if (gBallLayout == BALL_LAYOUT_DEFAULT) {
			float pos[4]  = { 0.0f, 0.0f, 1.0f, 0.0f };
			float ambient[4]  = { 0.2f, 0.2f, 0.2f, 1.0f };
			float diffuse[4]  = { 1.0f, 1.0f, 1.0f, 1.0f };
			float specular[4] = { 0.0f, 0.0f, 0.0f, 1.0f };

			glLightfv(GL_LIGHT0, GL_POSITION, pos);
			glLightfv(GL_LIGHT0, GL_AMBIENT,  ambient);
			glLightfv(GL_LIGHT0, GL_DIFFUSE,  diffuse);
			glLightfv(GL_LIGHT0, GL_SPECULAR, specular);
			glEnable(GL_LIGHT0);

			glEnable(GL_LIGHTING);
		}

		glPushMatrix();

			glTranslatef(sgoBall.pos.x, sgoBall.pos.y, sgoBall.pos.z);
			glScalef(sgoBall.radius, sgoBall.radius, sgoBall.radius);
			quaternionTransform(sgoBall.orientation);

			/* explosion? */
			if (gIsBallInPieces) {
				drawExplosion(shader);
			} else {
				drawBallObject(shader);
			}

		glPopMatrix();

		if (gBallLayout == BALL_LAYOUT_DEFAULT) {
			glDisable(GL_LIGHTING);
		}

	deactivateBallShader();
}
