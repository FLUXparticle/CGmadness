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

#include "callback.h"

#include "light.h"
#include "text.h"
#include "features.h"

#include "debug.h"

#include <GL/glew.h>
#include <GL/glut.h>

#include <stdio.h>
#include <math.h>

Viewport sgWindowViewport;

static RenderTarget gTargetWindow;

/*** Scene ***/

static funcUpdate gUpdate = NULL;

static int gSceneDirty = 1;

void setUpdateFunc(funcUpdate update) {
	gUpdate = update;
}

/*** Display ***/

static float gFPS = 0.0f;

static Matrix gProjectionText;

static List gRenderTargets = NULL;

void addRenderTarget(RenderTarget* target) {
	if (hasFramebuffer()) {
		target->enabled = 1;
		gRenderTargets = prependElement(gRenderTargets, target);
	}
}

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

void switchRenderTarget(RenderTarget* target) {
	if (hasFramebuffer()) {
		glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, target->framebuffer);
		if (target->framebuffer != 0) {
			glFramebufferTexture2DEXT(GL_FRAMEBUFFER_EXT, GL_COLOR_ATTACHMENT0_EXT, target->texTarget, target->texID, 0);	
			glReadBuffer(GL_COLOR_ATTACHMENT0_EXT);
		} else {
			glReadBuffer(GL_BACK);
		}
	}

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glViewport(0, 0, target->width, target->height);
}

void display(void) {
	List run;

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	for (run = gRenderTargets; run; run = run->next) {
		RenderTarget* target = run->info;
		Viewport* v = target->viewport;
		if (target->enabled && v) {
			float aspect = (float) target->height / target->width;

			switchRenderTarget(target);
			
			glMatrixMode(GL_PROJECTION);
			glLoadMatrixf(&v->projection[0][0]);
			glScalef(aspect, 1.0f, 1.0f);
			
			glMatrixMode(GL_MODELVIEW);
			glLoadMatrixf(&v->view[0][0]);

			glEnable(GL_LIGHTING);
			setLights();

			v->draw();
		}
	}

	if (hasFramebuffer()) {
		glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, 0);
	}
	glViewport(0, 0, gTargetWindow.width, gTargetWindow.height);

	/* draw framerate */
	glMatrixMode(GL_PROJECTION);
	glLoadMatrixf(&gProjectionText[0][0]);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	{
		char text[20];
		sprintf(text, "FPS: %4.1f", gFPS);
		glDisable(GL_LIGHTING);
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

	gRenderTargets = prependElement(gRenderTargets, &gTargetWindow);

	glutReshapeFunc(reshape);
	glutDisplayFunc(display);

	/* for tasting optimizations on fast linux machines */
#if 0
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
	glutTimerFunc(gMillis, timer, glutGet(GLUT_ELAPSED_TIME));
}

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
