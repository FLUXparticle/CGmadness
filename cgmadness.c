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

#include "main.h"
#include "game.h"

#include "keyboard.h"
#include "mouse.h"

#include "tools.h"

#include "features.h"

#include <GL/glew.h>
#include <GL/glut.h>

#include <stdio.h>
#include <stdlib.h>

#define FRAMES_PER_SECOND 60

static void usage(void)
{
	printf("cgmadness [hotseat-mode]\n");
	printf("usage: cgmadness <cgm-file>\n");
}

int main(int argc, char* argv[])
{
	message();

	assurePath(argv[0]);

	if (argc > 1)
	{
		setHotSeatLevel(argv[1]);
	}
	else
	{
		usage();
		exit(1);
	}

  glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH | GLUT_STENCIL);

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

	glEnable(GL_NORMALIZE);

  glEnable(GL_CULL_FACE);
  glCullFace(GL_BACK);

	/* ---- */

	if (!initMain()) {
		return 1;
	}
	
  startKeyboard();
  startMouse();

  startTimer(FRAMES_PER_SECOND);
  startDisplay();

  glutMainLoop();

  return 0;
}
