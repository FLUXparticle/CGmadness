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

#include "game.h"
#include "common.h"
#include "keyboard.h"
#include "mouse.h"

#include "main.h"

#include "features.h"

#include <GL/glew.h>
#include <GL/glut.h>

#include <stdio.h>

#define FRAMES_PER_SECOND 60

int main(int argc, char* argv[]) {
	message();

	sgLevel.size.x = -1;
	sgLevel.size.y = -1;
		
  glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_ALPHA | GLUT_DEPTH | GLUT_STENCIL);

  glutInitWindowSize(700, 500);
  glutInitWindowPosition(0, 0);

  glutInit(&argc, argv);

  if (!glutCreateWindow("CG Madness")) {
		printf("Could not create window :(\n");
    return 1;
	}

  glutFullScreen();

	/* ---- */
	
	glewInit();

	initFeatures(argc, argv);
	
	/* ---- */

  glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClearStencil(0);

	glEnable(GL_DEPTH_TEST);
	
	/* per-vertex-fog */
	glHint(GL_FOG_HINT, GL_FASTEST);

	glEnable(GL_NORMALIZE);

  glEnable(GL_CULL_FACE);
  glCullFace(GL_BACK);

	/* ---- */

	if (!initGame()) {
		return 1;
	}
	
  startKeyboard();
  startMouse();

  startTimer(FRAMES_PER_SECOND);
  startDisplay();
	
  glutMainLoop();

  return 0;
}
