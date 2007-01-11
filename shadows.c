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

static GLhandleARB gSpotlightShader = 0;

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
 * WARNING Die Lichtquelle muss bei der Projektion immer über dem Punkt liegen,
 * sonst geht die Projektion in die flasche Richtung
 */
Vector3 projection(Vector3 light, Vector3 p) {
	Vector3 diff = norm(sub(p, light));
	return add(light, scale(-(light.z + 1.0f) / diff.z, diff));
}

/*
 * Berechnet die Shadow-Volumes für das aktuelle Level
 */
void initShadowVolumes(void) {
	Vector3 light;

	int p;
	int q;
	int i;

	glGetIntegerv(GL_STENCIL_BITS, &gStencilBits);

  /* Initialisierungen */ 
	gMaxFieldShadowVolumeQuads = 6 * sgMaxQuads;
	gMaxFieldShadowVolumeVertices = 4 * gMaxFieldShadowVolumeQuads;

	gCntFieldShadowVolume = 0;
	MALLOC(gFieldShadowVolume, gMaxFieldShadowVolumeVertices * sizeof(Vector3));

	for (i = 0; i <= BALL_SHADOW_VERTICES; i++) {
		gBallOutline[i].x = BALL_RADIUS * cos(2.0f * i * PI / BALL_SHADOW_VERTICES);
		gBallOutline[i].y = BALL_RADIUS * sin(2.0f * i * PI / BALL_SHADOW_VERTICES);
	}
	
	light.x = sgLight[0].pos[0];
	light.y = sgLight[0].pos[1];
	light.z = sgLight[0].pos[2];

	/* Spielfeldschatten */
	
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
					 * Wenn d < 0, dann liegt diese Fläche im Schatten und es reicht, wenn solche Flächen ein Shadow-Volume bekommen.
					 */
					if (d < 0) {
						shadow = 1;
					}
					/*
					 * Flächen, die flach auf dem Boden liegen, brauchen auch kein Shadow-Volume.
					 */
					if (sgVertices[q + i].z > 0) {
						needed = 1;
					}
				}

				if (needed && shadow) {
					Vector3 vFar[4];

					/* Rückwand berechnen */
					for (i = 0; i < 4; i++) {
						vFar[i] = projection(light, sgVertices[q + i]);
					}

					/* Deckel */
					for (i = 3; i >= 0; i--) {
						gFieldShadowVolume[gCntFieldShadowVolume++] = sgVertices[q + i];
					}
				
					/* Seitenflächen */	
					for (i = 0; i < 4; i++) {
						int j = (i + 1) % 4;
						gFieldShadowVolume[gCntFieldShadowVolume++] = vFar[i];
						gFieldShadowVolume[gCntFieldShadowVolume++] = sgVertices[q + i];
						gFieldShadowVolume[gCntFieldShadowVolume++] = sgVertices[q + j];
						gFieldShadowVolume[gCntFieldShadowVolume++] = vFar[j];
					}

					/* Rückwand */
					for (i = 0; i < 4; i++) {
						gFieldShadowVolume[gCntFieldShadowVolume++] = vFar[i];
					}
				}

				q += 4;
			}	
		}
	}

	/*
	 * Shadow-Volumes in den Speicher der Grafikkarte laden
	 */
	if (hasVertexbuffer()) {
		glGenBuffersARB(1, &gVBufferFieldSchadowVolume);

		glBindBufferARB(GL_ARRAY_BUFFER_ARB, gVBufferFieldSchadowVolume);
		glBufferDataARB(GL_ARRAY_BUFFER_ARB, sizeof(Vector3) * gCntFieldShadowVolume, gFieldShadowVolume, GL_STATIC_DRAW_ARB);
	}

	printf("MAX_FIELD_SHADOW_VOLUME_VERTICES: %d\n", gMaxFieldShadowVolumeVertices);
	printf("gCntFieldShadowVolume: %d\n", gCntFieldShadowVolume);

	/*
	 * Spotlight-Shader laden, falls noch nicht geschehen
	 */
	if (hasShader() && !gSpotlightShader) {
		gSpotlightShader = makeShader("spotlight.vert", "spotlight.frag");
		if (gSpotlightShader) {
			printf("Spotlight-Shader ready :-)\n");
		}
	}
}

/*
 * Gibt den Speicherplatz wieder frei, der für Shadow-Volumes reserviert wurde.
 */
void destroyShadowVolumes(void) {
	if (hasVertexbuffer()) {
		glDeleteBuffersARB(1, &gVBufferFieldSchadowVolume);
  }
	FREE(gFieldShadowVolume);
}

/*
 * Rendert die Shadow-Volunmes des Spielfeldes
 */
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

/*
 * Rendert alle Shadow-Volumes
 */
void renderShadowVolumes(void) {
	int i;

	/* Schatten des Balls */

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

	/* Schatten des Spielfeldes */

	drawFieldShadowVolume();
}

/*
 * Passt das Shadow-Volume des Balls an
 */
void updateShadows(void) {
	int i;
	int cntVertices = 0;

	Vector3 light;
	Vector3 ball;
	Vector3 up = { 0.0f, 0.0f, 1.0f };
	Vector3 right;
	Vector3 diff;

	light.x = sgLight[0].pos[0];
	light.y = sgLight[0].pos[1];
	light.z = sgLight[0].pos[2];

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

/*
 * Füllt den gesammten Framebuffer. Dadurch kann z.B. die BlendingFunction auf den gesammten Framebuffer oder 
 * auch nur auf den durch den StencilBuffer freigegebenen Bereich angewendet werden.
 */
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

/*
 * Kümmert sichn um das Rendern des Spielfeldes incl. Schatten, Spotlight und Ball
 */
void drawShadows(int shouldDrawBall) {
  int stencilSpotlightMask = 1 << (gStencilBits - 1);
  int stencilShadowMask = stencilSpotlightMask - 1;
  int stencilFullMask = stencilSpotlightMask | stencilShadowMask;

	if (sgRenderPass > 0) {
		glStencilMask(stencilFullMask);
		glClearStencil(0);
		glClear(GL_STENCIL_BUFFER_BIT);

		if (useShadows()) {
			glDisable(GL_LIGHTING);
			glColorMask(0,0,0,0);

			/*
			 * Z-Buffer und Stencil-Buffer für den Ball updaten.
			 * Der Ball muss leider später durch den Stencil-Buffer neu
			 * gezeichnet werden, da es sonst zu Z-Fighting kommt.
			 */
			if (shouldDrawBall) {
				drawGameBall();
			}

			/* Shadow-Volume in den Stencil-Buffer rendern */

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

				/* Rückseiten zeichen ... */
				glCullFace(GL_FRONT);
				/* ... und dabei runterzählen */
				glStencilOp(GL_KEEP, GL_INCR, GL_KEEP);

					renderShadowVolumes();
				
				/* Vorderseiten zeichen ... */
				glCullFace(GL_BACK);
				/* ... und dabei hochzählen */
				glStencilOp(GL_KEEP, GL_DECR, GL_KEEP);

					renderShadowVolumes();
			}

			/* Scene an den Stellen, wo Schatten ist, ohne Licht rendern */
			glStencilOp(GL_KEEP, GL_KEEP, GL_KEEP);

			/* Schattenbereiche in den Alpha-Kanal eintragen */
			glColorMask(0,0,0,1);
			glStencilFunc(GL_NOTEQUAL, 0, stencilShadowMask);

				glColor4f(0.0f, 0.0f, 0.0f, 0.4f);
				fillWholeFramebuffer();
		}
		
		/* Alles mit dem Alpha-Kanel blenden */
		glEnable(GL_BLEND);
		glColorMask(1,1,1,1);
		glBlendFunc(GL_DST_COLOR, GL_DST_ALPHA);

		glEnable(GL_STENCIL_TEST);
		glStencilMask(stencilSpotlightMask);
		glStencilFunc(GL_ALWAYS, stencilSpotlightMask, stencilSpotlightMask);
		glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);

		/*
		 * Zeichnet den Bereich des Spielfeldes, ob den das Spotlight fällt mit aktiviertem Spotlight-Shader und 
		 * markiert die bereits gezeichneten Pixel im StencilBuffer.
		 */
		if (useSpotlight()) {
			glUseProgram(gSpotlightShader);

			glDepthFunc(GL_EQUAL);

				drawGameFieldSpotlightParts();

			glUseProgram(0);
		}

		/*
		 * Der restliche Bereich des Framebuffers wird mittels Alpha-Blending auf die richtige Hilligkeit gebracht.
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
		/* Ball */

		glEnable(GL_LIGHTING);
		glColorMask(1,1,1,1);

		glEnable(GL_DEPTH_TEST);
		glDepthMask(1);
		glDepthFunc(GL_LESS);

		glEnable(GL_POLYGON_OFFSET_FILL);
		glPolygonOffset(0, -1);

		if (shouldDrawBall) {
			activateBallShader();

			/* Ball im Licht */

			glStencilFunc(GL_EQUAL, 0, stencilShadowMask);

				drawGameBall();

			/* Ball im Schatten */

			toggleLight(sgGameMainLight);

			glStencilFunc(GL_NOTEQUAL, 0, stencilShadowMask);

				drawGameBall();

			toggleLight(sgGameMainLight);

			deactivateBallShader();
			
			glDisable(GL_STENCIL_TEST);
		}
		glDisable(GL_POLYGON_OFFSET_FILL);
	}

	/* Normalen Renderzustand wieder herstellen */

	glDisable(GL_BLEND);

	glEnable(GL_LIGHTING);
		
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);
	glDepthMask(1);
	
	glColorMask(1,1,1,1);

	glDisable(GL_STENCIL_TEST);
}
