/*
 * CG Madness - a Marble Madness clone
 * Copyright (C) 2007  Sven Reinck <sreinck@gmx.de>
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

#include "callback.h"

#include "text.h"

#include "debug.h"

#include <GL/glew.h>
#include <GL/glut.h>

#include <stdio.h>
#include <math.h>

#define DEBUG_PREDISPLAY 0
#define NO_FRAME_LIMIT 0

Viewport sgWindowViewport;

static RenderTarget gTargetWindow;

/*** Scene ***/

static funcUpdate gUpdate     = NULL;
static funcDraw   gPreDisplay = NULL;

static int gSceneDirty = 1;

void setUpdateFunc(funcUpdate update) {
	gUpdate = update;
}

void setPreDisplayFunc(funcDraw preDisplay) {
	gPreDisplay = preDisplay;
}

/*** Display ***/

static float gFPS = 0.0f;

static Matrix gProjectionText;

void framerate(void) {
  static int timebase = 0;
  static int frameCount = 0;
  static int time = 0;

  frameCount++;

  time = glutGet(GLUT_ELAPSED_TIME);

  if (time - timebase > 1000) {
		gFPS = frameCount * 1000.0 / (time - timebase);

		timebase = time;
		frameCount = 0;
	}
}

void display(void) {
#if(DEBUG_PREDISPLAY)
	static int sumPredisplayTime = 0;
	static int cntPredisplayTime = 0;
	float predisplayTime = 0.0f;
#endif
	if (gPreDisplay) {
#if(DEBUG_PREDISPLAY)
		int after;
		int before = glutGet(GLUT_ELAPSED_TIME);
#endif
		gPreDisplay();
#if(DEBUG_PREDISPLAY)
		after = glutGet(GLUT_ELAPSED_TIME);
		sumPredisplayTime += after - before;
		cntPredisplayTime++;
		predisplayTime = (float) sumPredisplayTime / glutGet(GLUT_ELAPSED_TIME);
#endif
	}

	glViewport(0, 0, gTargetWindow.width, gTargetWindow.height);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

	{
		Viewport* v = gTargetWindow.viewport;
		float aspect = (float) gTargetWindow.height / gTargetWindow.width;

		glMatrixMode(GL_PROJECTION);
		glLoadMatrixf(&v->projection[0][0]);
		glScalef(aspect, 1.0f, 1.0f);

		glMatrixMode(GL_MODELVIEW);
		glLoadMatrixf(&v->view[0][0]);

		v->draw();
	}

	/* draw framerate */
	glMatrixMode(GL_PROJECTION);
	glLoadMatrixf(&gProjectionText[0][0]);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	{
#if(DEBUG_PREDISPLAY)
		char text[48];
		sprintf(text, "FPS: %4.1f PredisplayTime: %4.1f%%", gFPS, predisplayTime * 100.0f);
#else
		char text[20];
		sprintf(text, "FPS: %4.1f", gFPS);
#endif
		glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
		glRasterPos2f(0.0f, 0.0f);
		drawBitmapText(text);
	}

	glutSwapBuffers();
	framerate();

	gSceneDirty = 0;
}

void reshape(int w, int h) {
	gTargetWindow.width = w;
	gTargetWindow.height = h;
}

void startDisplay(void) {
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(0.0f, 1.0f, 0.0f, 1.0f, 0.0f, 1.0f);
	glGetFloatv(GL_PROJECTION_MATRIX, &gProjectionText[0][0]);

	{
		int x;
		int y;
		Viewport* v = &sgWindowViewport;

		for (x = 0; x < 4; x++) {
			for (y = 0; y < 4; y++) {
				v->view[x][y] = (x == y);
			}
		}

		initProjectMat(v->projection, FOV);
	}

	/* RenderTarget for main window */
	gTargetWindow.enabled = 1;
	gTargetWindow.framebuffer = 0;
	gTargetWindow.viewport = &sgWindowViewport;

	glutReshapeFunc(reshape);
	glutDisplayFunc(display);

	/* for testing optimizations on fast linux machines */
#if (NO_FRAME_LIMIT)
	glutIdleFunc(display);
#endif
}

/*** Update ***/

int gMillis;

void timer(int lastCallTime) {
  int thisCallTime = glutGet(GLUT_ELAPSED_TIME);
	int lastUpdateTime = lastCallTime;
	int nextUpdateTime = lastUpdateTime + gMillis;
	int diff;

	while (nextUpdateTime < thisCallTime) {
		float interval = (float) (nextUpdateTime - lastUpdateTime) / 1000.0f;
		gUpdate(interval);
		lastUpdateTime = nextUpdateTime;
		nextUpdateTime += gMillis;
	}

	diff = nextUpdateTime - glutGet(GLUT_ELAPSED_TIME);
	if (diff < 0) {
		diff = 0;
	}
	glutTimerFunc(diff, timer, lastUpdateTime);

	if (!gSceneDirty) {
		gSceneDirty = 1;
		glutPostRedisplay();
	}
}

void startTimer(int callsPerSecond) {
  gMillis = 1000 / callsPerSecond;
	timer(glutGet(GLUT_ELAPSED_TIME));
}

/*** Picking ***/

int pick(int x, int y) {
  int viewport[4];
	float aspect;

	int width  = gTargetWindow.width;
	int height = gTargetWindow.height;

	Viewport* v = gTargetWindow.viewport;

	viewport[0] = 0;
	viewport[1] = 0;
	viewport[2] = width;
	viewport[3] = height;
	aspect = (float) height / width;

	glRenderMode(GL_SELECT);

	glInitNames();

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	gluPickMatrix(x, height - y, 5, 5, viewport);

	glMultMatrixf(&v->projection[0][0]);
	glScalef(aspect, 1.0f, 1.0f);

	glMatrixMode(GL_MODELVIEW);
	glLoadMatrixf(&v->view[0][0]);
	gTargetWindow.viewport->pick();
	glFlush();

	return glRenderMode(GL_RENDER);
}

void centerMouse(int* x, int* y)
{
	*x = gTargetWindow.width / 2;
	*y = gTargetWindow.height / 2;
	glutWarpPointer(*x, *y);
}
