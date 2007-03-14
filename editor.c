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
 
#include "editor.h"

#include "common.h"
#include "editormenu.h"

#include "graph.h"
#include "light.h"
#include "callback.h"
#include "keyboard.h"

#include "functions.h"

#include "types.h"
#include "debug.h"

#include <GL/glut.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#define MAX_LEVEL_SIZE 100
#define MAX_LEVEL_HIGHT 100

static int gEditorMainLight;

static int gIsEditorRunning;

static FieldCoord gCurStart;
static FieldCoord gCurEnd;
static int gCamAngle = 0;

static int gSin[] = { 0, 1, 0, -1 };
static int gCos[] = { 1, 0, -1, 0 };

static char* gFilename;

static Vector3 gEditorMenuPosition;

void pauseEditor(void) {
	showEditorMenu(0);
	gIsEditorRunning = 0;
}

void resumeEditor(void) {
	glutSetCursor(GLUT_CURSOR_NONE);
	gIsEditorRunning = 1;
}

void saveLevel(void) {
	if (saveFieldToFile(gFilename)) {
		showEditorMenu(1);
	} else {
		showEditorMenu(2);
	}
}

void updateEditorCamera(float interval, Vector3 ball) {
	static float distance = 5.0f;
	static float height = 2.0f;
	static Vector3 dest = { 0.0f, 0.0f, 0.0f };
	float angle;
	Vector3 marker;

  /* camera controls for editor */

	/* zoom */
	if (isKeyPressed('v')) distance += 0.1f;
	if (isKeyPressed('c') && distance > 0.5) distance -= 0.1f;

	/* rotation */
	if (wasKeyPressed('b')) gCamAngle--; 
	if (wasKeyPressed('n')) gCamAngle++;

	if (gCamAngle <  0) gCamAngle += 4;
	if (gCamAngle >= 4) gCamAngle -= 4;

	/* height */
	if (isKeyPressed('x')) height += 0.1f;
	if (isKeyPressed('y')) height -= 0.1f;
	
	/* look at */
	marker.x = (gCurStart.x + gCurEnd.x) / 2.0f + 0.5f;
	marker.y = (gCurStart.y + gCurEnd.y) / 2.0f + 0.5f;
	marker.z = ball.z;

	angle = gCamAngle * 90.0f;

	/* new camera position */
	dest.x =  sin(angle * PI / 180.0f) * distance + marker.x;
	dest.y = -cos(angle * PI / 180.0f) * distance + marker.y;
	dest.z = ball.z + height;

	moveCamera(interval, dest, marker);

	sgLight[0].pos = sgCamera;
}

void changeMarkerArea(int incz, int incdzx, int incdzy) {
	int x;
	int y;
	int incx;
	int incy;
	
	incx = -(gCurEnd.x - gCurStart.x) * incdzx;
	
	for (x = gCurStart.x; x <= gCurEnd.x; x++) {
		incy = -(gCurEnd.y - gCurStart.y) * incdzy;
		
		for (y = gCurStart.y; y <= gCurEnd.y; y++) {
			Plate* p = &sgLevel.field[x][y];
  		int z = p->z;
			int dzx = p->dzx;
			int dzy = p->dzy;
			
			z += incz + incx + incy;
			dzx += incdzx;
			dzy += incdzy;
			
			if (z - (abs(dzx) + abs(dzy)) >= 0 && z + (abs(dzx) + abs(dzy)) <= MAX_LEVEL_HIGHT)  {
				p->z = z;
				p->dzx = dzx;
				p->dzy = dzy;
			}

			incy += 2 * incdzy;
		}
		
		incx += 2 * incdzx;
	}
}

void modBetween(int* value, int mod, int min, int max) {
	*value += mod;

	if (*value < min) {
		*value = min;
	} else if (*value >= max) {
		*value = max - 1;
	}
}

void moveMarker(int markermode, int dx, int dy) {
	if (markermode) {
		modBetween(&gCurEnd.x, dx, gCurStart.x, sgLevel.size.x);
		modBetween(&gCurEnd.y, dy, gCurStart.y, sgLevel.size.y);
	} else {
		int sx = gCurEnd.x - gCurStart.x;
		int sy = gCurEnd.y - gCurStart.y;

		modBetween(&gCurStart.x, dx, 0, sgLevel.size.x - sx);
		modBetween(&gCurStart.y, dy, 0, sgLevel.size.y - sy);

		gCurEnd.x = gCurStart.x + sx;
		gCurEnd.y = gCurStart.y + sy;
	}
}

void moveMarkerLeft(int markermode) {
	moveMarker(markermode, -gCos[gCamAngle], -gSin[gCamAngle]);
}

void moveMarkerRight(int markermode) {
	moveMarker(markermode, gCos[gCamAngle], gSin[gCamAngle]);
}

void moveMarkerUp(int markermode) {
	moveMarker(markermode, -gSin[gCamAngle], gCos[gCamAngle]);
}

void moveMarkerDown(int markermode) {
	moveMarker(markermode,  gSin[gCamAngle], -gCos[gCamAngle]);
}

void animateEditor(float interval) {
	int markermode = 0;

	/* editor controls for changing environment */	
	if (wasKeyPressed('a')) {
		changeMarkerArea(0, gCos[gCamAngle], gSin[gCamAngle]);
	}
	
	if (wasKeyPressed('d')) {
		changeMarkerArea(0, -gCos[gCamAngle], -gSin[gCamAngle]);
	}

	
	if (wasKeyPressed('s')) {
		changeMarkerArea(0, -gSin[gCamAngle], gCos[gCamAngle]);
	}
	
	if (wasKeyPressed('w')) {
		changeMarkerArea(0, gSin[gCamAngle], -gCos[gCamAngle]);
  }
	
	if (wasKeyPressed('f'))  {
		changeMarkerArea(-1, 0, 0);
	}
	
	if (wasKeyPressed('r')) {
		changeMarkerArea(1, 0, 0)	;
	}
	
	/* editor controls for current field */
	if (wasKeyPressed('1')) {
		if (gCurStart.x != sgLevel.finish.x || gCurStart.y != sgLevel.finish.y) {
			sgLevel.start.x = gCurStart.x;
			sgLevel.start.y = gCurStart.y;
		}
	}
	
	if (wasKeyPressed('2')) {
		if (gCurStart.x != sgLevel.start.x || gCurStart.y != sgLevel.start.y) {
			sgLevel.finish.x = gCurStart.x;
			sgLevel.finish.y = gCurStart.y;
	  }
	}
	
	markermode = getModifiers() == GLUT_ACTIVE_SHIFT;

	/*  editor controls for moving selection */
	if (wasCursorPressed(CURSOR_LEFT)) {
		moveMarkerLeft(markermode);
	}
	
	if (wasCursorPressed(CURSOR_RIGHT)) {
		moveMarkerRight(markermode);
	}
	
	if (wasCursorPressed(CURSOR_DOWN)) {
		moveMarkerDown(markermode);
	}
	
	if (wasCursorPressed(CURSOR_UP)) {
		moveMarkerUp(markermode);
	}
}

void updateEditor(float interval) {
	if (gIsEditorRunning) {
		Vector3 markerPos;
  
		if (wasKeyPressed(KEY_ESC)) {
			pauseEditor();
		}

		markerPos.x = gCurStart.x + 0.5f;
		markerPos.y = gCurStart.y + 0.5f;
		markerPos.z = SCALE_Z * sgLevel.field[gCurStart.x][gCurStart.y].z;

		updateEditorCamera(interval, markerPos);
		animateEditor(interval);
	} else {
		/* show menu */
		Vector3 camera = gEditorMenuPosition;
		Vector3 lookat = gEditorMenuPosition;
		
		camera.y -= 10.0f;
		camera.z += 7.0f;

		lookat.z += 5.0f;

		updateEditorMenu(interval);

		moveCamera(interval, camera, lookat);
		sgLight[0].pos = sgCamera;
	}
}

void drawEditorField(void) {
	Square square;
	int i, j;
	FieldCoord cur;
	
	glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, sgLevel.texture);

		glBegin(GL_QUADS);
		
		for (cur.x = 0; cur.x < sgLevel.size.x; cur.x++) {
			for (cur.y = 0; cur.y < sgLevel.size.y; cur.y++) {
				
				if (cur.x >= gCurStart.x && cur.x <= gCurEnd.x && cur.y <= gCurEnd.y && cur.y >= gCurStart.y) {
					setAttributes(1.0f, 0.0f, 0.0f, 0.2f, 0.8f, 0.0f);
				} else if (cur.x == sgLevel.start.x && cur.y == sgLevel.start.y) {
					setAttributes(0.0f, 1.0f, 0.0f, 0.2f, 0.8f, 0.0f);
				} else if (cur.x == sgLevel.finish.x && cur.y == sgLevel.finish.y) {
					setAttributes(0.0f, 0.0f, 1.0f, 0.2f, 0.8f, 0.0f);
				} else {
					setAttributes(1.0f, 1.0f, 1.0f, 0.2f, 0.8f, 0.0f);
				}
				
				getRoofSquare(cur.x, cur.y, &square);
	
				glNormal3fv(&square.normal.x);
				for (i = 0; i < 4; i++) {
					glTexCoord2fv(&square.texcoords[i].x);
					glVertex3fv(&square.vertices[i].x);
				}
	
				for (j = 0; j < 4; j++) {
					if (getSideSquare(cur.x, cur.y, j, &square)) {
						glNormal3fv(&square.normal.x);
						for (i = 0; i < 4; i++) {
							glTexCoord2fv(&square.texcoords[i].x);
							glVertex3fv(&square.vertices[i].x);
						}
					}
				}
			}
		}	
	  glEnd();
  
  glDisable(GL_TEXTURE_2D);
}

void drawEditor(void) {
	drawEditorField();

	if (!gIsEditorRunning)	{
		drawEditorMenu();
	}
}

void pickEditor(void) {
	if (!gIsEditorRunning)	{
		pickEditorMenu();
	}
}

int initEditor(char* filename) {
	glColor3f(1.0f, 1.0f, 1.0f);
	gEditorMainLight = addPointLight(0.0f, 0.0f, 0.0f);

	gFilename = filename;
	if (!loadFieldFromFile(gFilename)) {
		if (between(sgLevel.size.x, 0, MAX_LEVEL_SIZE) && between(sgLevel.size.y, 0, MAX_LEVEL_SIZE)) {
			newLevel();
		} else {
			return 0;
		}
	}

	sgWindowViewport.draw = drawEditor;
	sgWindowViewport.pick = pickEditor;
	setUpdateFunc(updateEditor);

	gCurStart.x = 0;
	gCurStart.y = 0;
	gCurEnd.x = 0;
	gCurEnd.y = 0;

	initEditorMenu();

	gEditorMenuPosition.x = sgLevel.size.x / 2.0f;
	gEditorMenuPosition.y = -10.0f;
	gEditorMenuPosition.z =   0.0f;
	
	setEditorMenuPosition(gEditorMenuPosition);

	pauseEditor();
		
	return 1;
}
