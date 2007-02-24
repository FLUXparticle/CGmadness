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

#include "shadows.h"

#include "light.h"
#include "ball.h"
#include "game.h"
#include "common.h"
#include "field.h"

#include "objects.h"
#include "vector.h"
#include "shader.h"
#include "features.h"

#include "types.h"
#include "debug.h"

#include <GL/glew.h>

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define BALL_SHADOW_VERTICES 20

#define BALL_SHADOW_VOLUME_VERTICES ((BALL_SHADOW_VERTICES + 1) * 2)

static Vector2 gBallOutline[BALL_SHADOW_VERTICES + 1];

static int gCntFieldShadowVolume;
static Vector3* gFieldShadowVolume;
static unsigned int gVBufferFieldSchadowVolume;

static Vector3 gBallShadowVolume[BALL_SHADOW_VOLUME_VERTICES];

static int gMaxFieldShadowVolumeQuads;
static int gMaxFieldShadowVolumeVertices;

static int gStencilBits;

typedef struct {
	Vector3 corners[8];
	
	struct {
		int corners[2];
		int faces[2];
	} edges[12];
	
	struct {
		int corners[4];
		int inShadow;
	} faces[6];
} Cubic;

/*
 * WARNING: light source must be heigher than projected point
 */
Vector3 projection(Vector3 light, Vector3 p) {
	Vector3 diff = norm(sub(p, light));
	return add(light, scale(-(light.z + 1.0f) / diff.z, diff));
}

void initShadowVolumes(void) {
	Vector3 light;

	int p;
	int q;
	int i;

	glGetIntegerv(GL_STENCIL_BITS, &gStencilBits);

  /* init */ 
	gMaxFieldShadowVolumeQuads = 6 * sgMaxQuads;
	gMaxFieldShadowVolumeVertices = 4 * gMaxFieldShadowVolumeQuads;

	gCntFieldShadowVolume = 0;
	MALLOC(gFieldShadowVolume, gMaxFieldShadowVolumeVertices * sizeof(Vector3));

	for (i = 0; i <= BALL_SHADOW_VERTICES; i++) {
		gBallOutline[i].x = sgoBall.radius * cos(2.0f * i * PI / BALL_SHADOW_VERTICES);
		gBallOutline[i].y = sgoBall.radius * sin(2.0f * i * PI / BALL_SHADOW_VERTICES);
	}
	
	light = sgLight[0].pos;

	/* field shadows */
	q = 0;
	for (p = 0; p < sgMaxPlates; p++) {
		int k;
		for (k = 0; k < 5; k++) {
			if (!sgQuadInShadow[p * 5 + k]) {
				int needed = 0;
				int shadow = 0;
				for (i = 0; i < 4 ; i++) {
					float d = dot(sub(light, sgVertices[q + i]), sgNormals[q + i]);
					/*
					 * if d < 0, plate is in shadow and only those need a shadow volume
					 */
					if (d < 0) {
						shadow = 1;
					}
					/*
					 * plates, that lie on the ground does not need a shadow volume
					 */
					if (sgVertices[q + i].z > 0) {
						needed = 1;
					}
				}

				if (needed && shadow) {
					Vector3 vFar[4];

					for (i = 0; i < 4; i++) {
						vFar[i] = projection(light, sgVertices[q + i]);
					}

					for (i = 3; i >= 0; i--) {
						gFieldShadowVolume[gCntFieldShadowVolume++] = sgVertices[q + i];
					}
				
					for (i = 0; i < 4; i++) {
						int j = (i + 1) % 4;
						gFieldShadowVolume[gCntFieldShadowVolume++] = vFar[i];
						gFieldShadowVolume[gCntFieldShadowVolume++] = sgVertices[q + i];
						gFieldShadowVolume[gCntFieldShadowVolume++] = sgVertices[q + j];
						gFieldShadowVolume[gCntFieldShadowVolume++] = vFar[j];
					}

					for (i = 0; i < 4; i++) {
						gFieldShadowVolume[gCntFieldShadowVolume++] = vFar[i];
					}
				}

				q += 4;
			}	
		}
	}

	if (hasVertexbuffer()) {
		glGenBuffersARB(1, &gVBufferFieldSchadowVolume);

		glBindBufferARB(GL_ARRAY_BUFFER_ARB, gVBufferFieldSchadowVolume);
		glBufferDataARB(GL_ARRAY_BUFFER_ARB, sizeof(Vector3) * gCntFieldShadowVolume, gFieldShadowVolume, GL_STATIC_DRAW_ARB);
	}

	printf("MAX_FIELD_SHADOW_VOLUME_VERTICES: %d\n", gMaxFieldShadowVolumeVertices);
	printf("gCntFieldShadowVolume: %d\n", gCntFieldShadowVolume);
}

void destroyShadowVolumes(void) {
	if (hasVertexbuffer()) {
		glDeleteBuffersARB(1, &gVBufferFieldSchadowVolume);
  }
	FREE(gFieldShadowVolume);
}

void drawFieldShadowVolume(void) {
	glEnableClientState(GL_VERTEX_ARRAY);

	if (hasVertexbuffer()) {
		glBindBufferARB(GL_ARRAY_BUFFER_ARB, gVBufferFieldSchadowVolume);
		glVertexPointer(3, GL_FLOAT, 0, NULL);
	} else {
		glVertexPointer(3, GL_FLOAT, 0, gFieldShadowVolume);
	}

		glDrawArrays(GL_QUADS, 0, gCntFieldShadowVolume);

	if (hasVertexbuffer()) {
		glBindBufferARB(GL_ARRAY_BUFFER_ARB, 0);
	}

	glDisableClientState(GL_VERTEX_ARRAY);
}

void renderShadowVolumes(void) {
	int i;

	/* ball shadow */
	glBegin(GL_POLYGON);
	for (i = BALL_SHADOW_VERTICES - 1; i >= 0; i--) {
		glVertex3fv(&gBallShadowVolume[2 * i + 1].x);
	}
	glEnd();

	glBegin(GL_POLYGON);
	for (i = 0 ; i < BALL_SHADOW_VERTICES; i++) {
		glVertex3fv(&gBallShadowVolume[2 * i].x);
	}
	glEnd();

	glEnableClientState(GL_VERTEX_ARRAY);
	
	glVertexPointer(3, GL_FLOAT, 0, gBallShadowVolume);

		glDrawArrays(GL_QUAD_STRIP, 0, BALL_SHADOW_VOLUME_VERTICES);
	
	glDisableClientState(GL_VERTEX_ARRAY);

	/* field shadow */

	drawFieldShadowVolume();
}

void updateShadows(void) {
	int i;
	int cntVertices = 0;

	Vector3 light;
	Vector3 ball;
	Vector3 up = { 0.0f, 0.0f, 1.0f };
	Vector3 right;
	Vector3 diff;

	light = sgLight[0].pos;

	ball = sgoBall.pos;

	diff = sub(light, ball);

	right = norm(cross(diff, up));

	up = norm(cross(right, diff));

	for (i = 0; i <= BALL_SHADOW_VERTICES; i++) {
		Vector3 vNear = add(ball, add(scale(gBallOutline[i].x, right), scale(gBallOutline[i].y, up)));
		Vector3 vFar  = projection(light, vNear);
		gBallShadowVolume[cntVertices++] = vFar;
		gBallShadowVolume[cntVertices++] = vNear;
	}
}

void fillWholeFramebuffer(void) {
		glMatrixMode(GL_PROJECTION);
		glPushMatrix();
		glLoadIdentity();
		
		glMatrixMode(GL_MODELVIEW);
		glPushMatrix();
		glLoadIdentity();

		glDisable(GL_DEPTH_TEST);

			drawSquare();
		
		glMatrixMode(GL_PROJECTION);
		glPopMatrix();
		
		glMatrixMode(GL_MODELVIEW);
		glPopMatrix();
		
		glEnable(GL_DEPTH_TEST);
}

void drawShadows(int shouldDrawBall) {
  int stencilSpotlightMask = 1 << (gStencilBits - 1);
  int stencilShadowMask = stencilSpotlightMask - 1;
  int stencilFullMask = stencilSpotlightMask | stencilShadowMask;

	if (sgRenderPass > 0) {
		glStencilMask(stencilFullMask);

		if (useShadows()) {
			glDisable(GL_LIGHTING);
			glColorMask(0,0,0,0);

			/*
			 * draw ball for z-buffer und stencil-buffer
			 */
			if (shouldDrawBall) {
				drawGameBall();
			}

			/* render shadow volumes to stencil buffer */
			glEnable(GL_STENCIL_TEST);

			glDepthMask(0);
			glDepthFunc(GL_LESS);

			glStencilFunc(GL_ALWAYS, 0, 0);

			if (hasTwoSideStencil()) {
				glDisable(GL_CULL_FACE);
				glEnable(GL_STENCIL_TEST_TWO_SIDE_EXT);

				glActiveStencilFaceEXT(GL_BACK);
				glStencilMask(stencilShadowMask);
				glStencilOp(GL_KEEP, GL_INCR_WRAP_EXT, GL_KEEP);
				
				glActiveStencilFaceEXT(GL_FRONT);
				glStencilMask(stencilShadowMask);
				glStencilOp(GL_KEEP, GL_DECR_WRAP_EXT, GL_KEEP);

					renderShadowVolumes();

				glEnable(GL_CULL_FACE);
				glDisable(GL_STENCIL_TEST_TWO_SIDE_EXT);
			} else {
				glStencilMask(stencilShadowMask);

				/* increment back-faces */
				glCullFace(GL_FRONT);
				glStencilOp(GL_KEEP, GL_INCR, GL_KEEP);

					renderShadowVolumes();
				
				/* decrement front-faces */
				glCullFace(GL_BACK);
				glStencilOp(GL_KEEP, GL_DECR, GL_KEEP);

					renderShadowVolumes();
			}

			/* render shadow parts without light */
			glStencilOp(GL_KEEP, GL_KEEP, GL_KEEP);

			/* write shadow parts to alpha-channel */
			glColorMask(0,0,0,1);
			glStencilFunc(GL_NOTEQUAL, 0, stencilShadowMask);

				glColor4f(0.0f, 0.0f, 0.0f, 0.4f);
				fillWholeFramebuffer();
		}
		
		/* blend everything with alpha channel */
		glEnable(GL_BLEND);
		glColorMask(1,1,1,1);
		glBlendFunc(GL_DST_COLOR, GL_DST_ALPHA);

		glEnable(GL_STENCIL_TEST);
		glStencilMask(stencilSpotlightMask);
		glStencilFunc(GL_ALWAYS, stencilSpotlightMask, stencilSpotlightMask);
		glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);

		/*
		 * render parts of field with spotlight and mark in stencil-buffer
		 */
		if (useSpotlight()) {
			glUseProgram(sgSpotlightShader);

				glDepthFunc(GL_EQUAL);

				drawGameFieldSpotlightParts();

			glUseProgram(0);
		}

		/*
		 * alpha blending to bring everything to correct brightness
		 */
		if (sgRenderPass > 1) {
			glStencilFunc(GL_EQUAL, 0, stencilSpotlightMask);
			glStencilOp(GL_KEEP, GL_KEEP, GL_KEEP);
			glBlendFunc(GL_ZERO, GL_DST_ALPHA);
			glColor4f(0.0f, 0.0f, 0.0f, 0.0f);
			fillWholeFramebuffer();
		}

		glDisable(GL_BLEND);
	}

	if (sgRenderPass > 2) {
		/* draw ball */

		glEnable(GL_LIGHTING);
		glColorMask(1,1,1,1);

		glEnable(GL_DEPTH_TEST);
		glDepthMask(1);
		glDepthFunc(GL_LESS);

		glEnable(GL_POLYGON_OFFSET_FILL);
		glPolygonOffset(0, -1);

		if (shouldDrawBall) {
			activateBallShader();

			/* ball in light */

			glStencilFunc(GL_EQUAL, 0, stencilShadowMask);

				drawGameBall();

			/* ball in shadow */

			disableLight(sgGameMainLight);

			glStencilFunc(GL_NOTEQUAL, 0, stencilShadowMask);

				drawGameBall();

			enableLight(sgGameMainLight);

			deactivateBallShader();
		}
		glDisable(GL_POLYGON_OFFSET_FILL);
	}

	/* back to normal */

	glDisable(GL_BLEND);

	glEnable(GL_LIGHTING);
		
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);
	glDepthMask(1);
	
	glColorMask(1,1,1,1);

	glDisable(GL_STENCIL_TEST);
	glStencilMask(stencilFullMask);
}
