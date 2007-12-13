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

#include "idle.h"

#include <GL/glut.h>

float sgIdleProgress;
int sgIdleWorking = 0;

static int gIdleStep;
static int gMaxIdleSteps;
static funcIdle gIdle;

static void doIdle(void)
{
	gIdle(gIdleStep);
	
	gIdleStep++;
	sgIdleProgress = (float) gIdleStep / gMaxIdleSteps;

	if (gIdleStep >= gMaxIdleSteps)
	{
		stopIdle();
	}
}

void startIdle(int steps, funcIdle idle)
{
	gIdleStep = 0;
	gMaxIdleSteps = steps;
	gIdle = idle;

	sgIdleProgress = 0.0f;
	sgIdleWorking = 1;

	glutIdleFunc(doIdle);
}

void stopIdle(void)
{
	sgIdleProgress = 1.0f;
	sgIdleWorking = 0;

	glutIdleFunc(NULL);
}
