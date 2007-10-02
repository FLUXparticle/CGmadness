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
#include "editor.h"
#include "common.h"

#include "keyboard.h"
#include "mouse.h"

#include <GL/glew.h>
#include <GL/glut.h>

#include <stdio.h>
#include <string.h>

#define FRAMES_PER_SECOND 60

void usage(void) {
	printf("usage: cgm-editor <cgm-file> [--size x y]\n");
}

int main(int argc, char* argv[]) {
	char* file = NULL;
	int i;

	message();

	assurePath(argv[0]);

	sgLevel.size.x = -1;
	sgLevel.size.y = -1;

	/* read parameters */
	for (i = 1; i < argc; ) {
		if (strcmp(argv[i], "--size") == 0 && i + 2 < argc) {
			i++;
			sgLevel.size.x = atoi(argv[i++]);
			sgLevel.size.y = atoi(argv[i++]);
		} else {
			file = argv[i++];
		}
	}

	if (!file) {
		usage();
		return 1;
	}

	/* --- */

  glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_ALPHA | GLUT_DEPTH | GLUT_STENCIL);

  glutInitWindowSize(700, 500);
  glutInitWindowPosition(0, 0);

  glutInit(&argc, argv);

  if (!glutCreateWindow("CGM-Editor")) {
		printf("Could not create window :(\n");
    return 1;
	}

  glutFullScreen();

	/* ---- */

	glewInit();

	/* ---- */

  glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

	glEnable(GL_DEPTH_TEST);

	glEnable(GL_NORMALIZE);

  glEnable(GL_CULL_FACE);
  glCullFace(GL_BACK);

	/* ---- */

  if (!initEditor(file)) {
		return 1;
	}

  startKeyboard();

  startMouse();

  startDisplay();

  startTimer(FRAMES_PER_SECOND);

  glutMainLoop();

  return 0;
}
