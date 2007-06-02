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

#include "environment.h"

#include "skysphere.h"
#include "skyplane.h"
#include "water.h"

#include <GL/gl.h>

void initEnvironment(void) {
 	initSkysphere();
 	initSkyplane(2000.0f, 180.0, 3 , 3, 0.85f, 40, 0);
	initWater();
}

void drawEnvironment(void) {
	glDisable(GL_DEPTH_TEST);
		drawSkysphere();
		drawSkyplane();
	glEnable(GL_DEPTH_TEST);

	drawWater();
}
