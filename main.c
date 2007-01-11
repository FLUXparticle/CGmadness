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

#include "editor.h"
#include "game.h"
#include "common.h"
#include "keyboard.h"
#include "mouse.h"

#include "features.h"

#include <GL/glew.h>
#include <GL/glut.h>

#include <stdio.h>
#include <string.h>

#define FRAMES_PER_SECOND 60

void usage(void) {
	printf("usage: cgmadness (--editor <level> (--size x y))\n");
}

/* Hauptprogramm */
int main(int argc, char* argv[]) {
	int mode = 0;
	char* file = "";
	int i;

	printf("CG Madness, Copyright (C) 2007 Sven Reinck\n");
	printf("CG Madness comes with ABSOLUTELY NO WARRANTY;\n");

	sgLevel.size.x = -1;
	sgLevel.size.y = -1;
		
	/* Parameter parsen */
	for (i = 1; i < argc; ) {
		if (strcmp(argv[i], "--editor") == 0) {
			i++;
			mode = 1;
			if (i < argc) {
				file = argv[i++];
			} else {
				usage();
				exit(1);
			}
		} else if (strcmp(argv[i], "--size") == 0) {
			i++;
			sgLevel.size.x = atoi(argv[i++]);
			sgLevel.size.y = atoi(argv[i++]);
		} else {
			mode = 2;
			i++;
		}
	}
  if (argc == 1) mode = 2;

	if (mode == 0) {
		usage();
		exit(1);
	}
	
	if (mode != 1) {
		sgLevel.size.x = -1;
		sgLevel.size.y = -1;
	}

	/* --- */

  glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_ALPHA | GLUT_DEPTH | GLUT_STENCIL);

  glutInitWindowSize(700, 500);
  glutInitWindowPosition(0, 0);

  /* Glut initialisieren */
  glutInit(&argc, argv);

  /* Erzeugen des Fensters */
  if (!glutCreateWindow("CG Madness")) {
		printf("Could not create window :(\n");
    return 1;
	}

  glutFullScreen();

	/* ---- */
	
	glewInit();

	initFeatures(argc, argv);
	
	/* ---- */

  /* Hintergrundfarbe setzen*/
  glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

	/* Z-Buffer aktivieren */
	glEnable(GL_DEPTH_TEST);
	
	/* ??? */
	glShadeModel(GL_SMOOTH);

	/* Per-Vertex-Fog */
/*	glHint(GL_FOG_HINT, GL_FASTEST); */

	/* Normalen normalisieren */
	glEnable(GL_NORMALIZE);

	/* Polygonrueckseiten nicht anzeigen */
  glEnable(GL_CULL_FACE);
  glCullFace(GL_BACK);

	/* ---- */

  /* Keyboardhandler */ 
  startKeyboard();

  /* Scene erstellen */
	switch (mode) {
	case 1:
	  initEditor(file);
		break;
	case 2:
		initGame(file);
		break;
	default:
		exit(1);
	}
	
  /* Die benötigten Zeichenfunktionen in GLUT registrieren */
  startDisplay();
	
  startMouse();

  /* Timerfunktion mit Wiedeholrate registrieren */ 
  startTimer(FRAMES_PER_SECOND);

  /* Hauptroutine */
  glutMainLoop();

  return 0;
}
