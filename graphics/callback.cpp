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

#include "graphics/callback.hpp"

#include "text/text.hpp"
#include "graphics/camera.hpp"

#include "hw/keyboard.hpp"

#include GLUT_H

#include <stdio.h>
#include <math.h>

#define FRAMES_PER_SECOND 60
#define TIME_STEP (1000 / FRAMES_PER_SECOND)

#define FOV 60.0f

#define DEBUG_PREDISPLAY 0
#define NO_FRAME_LIMIT 0

Viewport sgWindowViewport;

static RenderTarget gTargetWindow;

static Process* gProcess;

/*** Scene ***/

static bool gSceneDirty = 1;

/*** Display ***/

static float gFPS = 0.0f;

static Matrix gProjectionText;

static bool gFreeze = false;

static void framerate()
{
	static int timebase = 0;
	static int frameCount = 0;
	static int time = 0;

	frameCount++;

	time = glutGet(GLUT_ELAPSED_TIME);

	if (time - timebase > 1000)
	{
		gFPS = frameCount * 1000.0 / (time - timebase);

		timebase = time;
		frameCount = 0;
	}
}

static void drawBitmapText(const char *str)
{
	for (; *str; str++)
	{
		glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, *str);
	}
}

void setDefaultValues()
{
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

	glEnable(GL_DEPTH_TEST);

	glEnable(GL_NORMALIZE);

	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);

	{
		float ambient[4] = { 0.2f, 0.2f, 0.2f, 1.0f };
		glLightfv(GL_LIGHT0, GL_AMBIENT, ambient);
		glEnable(GL_LIGHT0);
	}

	glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);
	glEnable(GL_COLOR_MATERIAL);
}

static void display()
{
#if(DEBUG_PREDISPLAY)
	static int sumPredisplayTime = 0;
	static int cntPredisplayTime = 0;
	float predisplayTime = 0.0f;
#endif

	Viewport *v = gTargetWindow.viewport;
	float aspect = (float) gTargetWindow.height / gTargetWindow.width;

	{
#if(DEBUG_PREDISPLAY)
		int after;
		int before = glutGet(GLUT_ELAPSED_TIME);
#endif
		gProcess->preDisplay();
#if(DEBUG_PREDISPLAY)
		after = glutGet(GLUT_ELAPSED_TIME);
		sumPredisplayTime += after - before;
		cntPredisplayTime++;
		predisplayTime = (float) sumPredisplayTime / glutGet(GLUT_ELAPSED_TIME);
#endif
	}

	glViewport(0, 0, gTargetWindow.width, gTargetWindow.height);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	{
		glMatrixMode(GL_PROJECTION);
		glLoadMatrixf(&v->projection[0][0]);
		glScalef(aspect, 1.0f, 1.0f);

		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();
		setCamera();

		gProcess->draw();
	}

	/* draw framerate */
	glMatrixMode(GL_PROJECTION);
	glLoadMatrixf(&gProjectionText[0][0]);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	glDisable(GL_DEPTH_TEST);

	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	{
		glScalef(aspect, 1.0f, 1.0f);

		glMatrixMode(GL_MODELVIEW);
		{
			gProcess->drawHUD(1.0f / aspect, 1.0f);
		}
		glMatrixMode(GL_PROJECTION);
	}
	glPopMatrix();

	{
#if(DEBUG_PREDISPLAY)
		char text[48];
		sprintf(text, "FPS: %4.1f PredisplayTime: %4.1f%%", gFPS,
						predisplayTime * 100.0f);
#else
		char text[20];
		sprintf(text, "FPS: %4.1f", gFPS);
#endif
		glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
		glRasterPos2f(0.0f, 0.0f);
		drawBitmapText(text);
	}

	glEnable(GL_DEPTH_TEST);

	if (!gFreeze)
	{
		glutSwapBuffers();
	}
	framerate();

	gSceneDirty = false;
}

void reshape(int w, int h)
{
	gTargetWindow.width = w;
	gTargetWindow.height = h;
}

void startDisplay(void)
{
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(0.0f, 1.0f, 0.0f, 1.0f, 0.0f, 1.0f);
	glGetFloatv(GL_PROJECTION_MATRIX, &gProjectionText[0][0]);

	initProjectMat(sgWindowViewport.projection, FOV);

	/* RenderTarget for main window */
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

void timer(int startTime)
{
	static int simulationTime = 0;
	int realTime = glutGet(GLUT_ELAPSED_TIME) - startTime;

	if (!gSceneDirty)
	{
		gSceneDirty = true;
		glutPostRedisplay();
	}

	glutTimerFunc(gMillis, timer, startTime);

	while (simulationTime < realTime)
	{
		gProcess->update(TIME_STEP / 1000.0f);
		simulationTime += TIME_STEP;
	}

	if (wasFunctionPressed(12))
	{
		gFreeze = !gFreeze;
	}
}

void startTimer()
{
	setUpdateFrequency(0);
	timer(glutGet(GLUT_ELAPSED_TIME));
}

void setUpdateFrequency(int callsPerSecond)
{
	if (callsPerSecond > 0)
	{
		gMillis = 1000 / callsPerSecond;
	}
	else
	{
		gMillis = TIME_STEP;
	}
}

/*** Callback ***/

void startCallback(Process* process)
{
	gProcess = process;

	startTimer();
	startDisplay();
}

/*** Picking ***/

void centerMouse(int *x, int *y)
{
	*x = gTargetWindow.width / 2;
	*y = gTargetWindow.height / 2;
	glutWarpPointer(*x, *y);
}
