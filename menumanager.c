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
 */

#include "menumanager.h"

#include "idle.h"
#include "keyboard.h"
#include "callback.h"
#include "texture.h"
#include "camera.h"
#include "objects.h"

#include <GL/glut.h>

static GLuint gTexLogo = 0;

static Screen* gCurScreen = NULL;

static Screen gScreenWait;
static funcCallback gWaitCallback = NULL;

void initMenuManager(void)
{
	static ProgressBar pbProgress;

	static MenuItem* itemsWait[] =
	{
		&pbProgress.item
	};

	initGUI();

	if (gTexLogo == 0)
	{
		gTexLogo = loadTexture("data/logo.tga", 0);
	}
	
	/* wait menu */
	initProgressBar(&pbProgress, 5.0f, &sgIdleProgress);

	INIT_SCREEN(&gScreenWait, itemsWait);
	
	gCurScreen = NULL;
}

const Screen* getCurScreen(void)
{
	return gCurScreen; 
}

void updateMenuManager(float interval)
{
	Vector3 camera = vector3(0.0f, -10.0f, 7.0f);
	Vector3 lookat = vector3(0.0f, 0.0f, 5.0f);

	moveCamera(interval, camera, lookat);
	
	updateScreen(gCurScreen, interval);

	if (gCurScreen == &gScreenWait)
	{
		if (wasKeyPressed(KEY_ESC) || wasKeyPressed('q'))
		{
			stopIdle();
			setUpdateFrequency(0);
			popScreen();
		} else if (!sgIdleWorking)
		{
			setUpdateFrequency(0);
			popScreen();
			
			if (gWaitCallback)
			{
				gWaitCallback();
			}
		}
	}
}

void eventMenuManager(const Vector3* position, const Vector3* direction, MouseEvent event)
{
	Vector3 newPosition = *position;

	if (gCurScreen && newPosition.y < 0.0f && direction->y > 0.0f)
	{
		float t = -newPosition.y / direction->y;
		float x = newPosition.x + t * direction->x;
		float y = newPosition.z + t * direction->z;

		eventScreen(gCurScreen, x, y, event);
	}
}

static void drawLogo(void)
{
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, gTexLogo);
	{
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		{
			glPushMatrix();
			{
				glTranslatef(0.0f, 8.0f, 0.0f);
				glScalef(4.0f, 1.0f, 1.0f);

				drawSquare();
			}
			glPopMatrix();
		}
		glDisable(GL_BLEND);
	}
	glDisable(GL_TEXTURE_2D);
}

void drawMenuManager(void)
{
	glEnable(GL_LIGHTING);
	glEnable(GL_COLOR_MATERIAL);

		glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);

		glColor3f(1.0f, 1.0f, 1.0f);

		setSomeLight();

		glPushMatrix();
			glRotatef(90.0f, 1.0f, 0.0f, 0.0f);

			drawLogo();
			drawScreen(gCurScreen);
		glPopMatrix();

	glDisable(GL_LIGHTING);
	glDisable(GL_COLOR_MATERIAL);
}

void showScreen(Screen* newScreen)
{
	prepareScreen(gCurScreen);

	glutSetCursor(GLUT_CURSOR_LEFT_ARROW);
}

void pushWaitScreen(funcCallback callback)
{
	gWaitCallback = callback;
	pushScreen(&gScreenWait);
}

void pushScreen(Screen* newScreen)
{
	if (gCurScreen == &gScreenWait)
	{
		newScreen->back = gCurScreen->back;
		gCurScreen->back = newScreen;
	}
	else
	{
		newScreen->back = gCurScreen;
		gCurScreen = newScreen;
	}

	showScreen(gCurScreen);
}

void popScreen(void)
{
	gCurScreen = gCurScreen->back;

	if (gCurScreen)
	{
		showScreen(gCurScreen);
	}
}
