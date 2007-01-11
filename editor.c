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

#include "graph.h"
#include "light.h"
#include "callback.h"
#include "keyboard.h"

#include "types.h"
#include "debug.h"

#include <GL/glu.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

static int gEditorMainLight;

static int gIsEditorRunning;

static FieldCoord gCurStart;
static FieldCoord gCurEnd;
static int gCamAngle = 0;

static int gSin[] = { 0, 1, 0, -1 };
static int gCos[] = { 1, 0, -1, 0 };

static char* gFilename;

void updateEditorCamera(double interval, Vector3 ball) {
	static float distance = 5.0f;
	static float height = 2.0f;
	static Vector3 dest = { 0.0f, 0.0f, 0.0f };
	float angle;
	Vector3 marker;

  /* camera controls for editor */

	/* zoom */
	if (isKeyPressed('c')) distance += 0.1f;
	if (isKeyPressed('v') && distance > 0.5) distance -= 0.1f;

	/* rotation */
	if (wasKeyPressed('b')) gCamAngle--; 
	if (wasKeyPressed('n')) gCamAngle++;

	if (gCamAngle <  0) gCamAngle += 4;
	if (gCamAngle >= 4) gCamAngle -= 4;

	/* height */
	if (isKeyPressed('.')) height += 0.1f;
	if (isKeyPressed(',')) height -= 0.1f;
	
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
}

void changeMarkerAreaSingle(int incz, int incdzx, int incdzy) {
	int i;
	int j;
	for (i = gCurStart.x; i <= gCurEnd.x; i++) {
		for (j = gCurStart.y; j <= gCurEnd.y; j++) {
				Plate* p = &sgLevel.field[i][j];
				int z = p->z;
				int dzx = p->dzx;
				int dzy = p->dzy;
			
      	z += incz;
				dzx += incdzx;
				dzy += incdzy;
				
				if ((z - (abs(dzx) + abs(dzy)) >= 0) && (z - abs(dzx + dzy) >= 0))  {
					p->z = z;
					p->dzx = dzx;
					p->dzy = dzy;
				}
			}
		}
}

void changeMarkerArea(int incdzx, int incdzy) {
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
			
			z += incx + incy;
			dzx += incdzx;
			dzy += incdzy;
			
			if ((z - (abs(dzx) + abs(dzy)) >= 0) && (z - abs(dzx + dzy) >= 0))  {
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

void animateEditor(double interval) {
	static int markermode = 0;
	static int singlemode = 0;

	/* editor controls for changing environment */	
	if (wasKeyPressed('a')) {
		if (singlemode) {
			changeMarkerAreaSingle(0, gCos[gCamAngle], gSin[gCamAngle]);
		} else {
			changeMarkerArea(gCos[gCamAngle], gSin[gCamAngle]);
		}
	}
	
	if (wasKeyPressed('d')) {
		if (singlemode) {
			changeMarkerAreaSingle(0, -gCos[gCamAngle], -gSin[gCamAngle]);
		} else {
			changeMarkerArea(-gCos[gCamAngle], -gSin[gCamAngle]);
		}
	}

	
	if (wasKeyPressed('s')) {
		if (singlemode) {
			changeMarkerAreaSingle(0, -gSin[gCamAngle], gCos[gCamAngle]);
		} else {
			changeMarkerArea(-gSin[gCamAngle], gCos[gCamAngle]);
		}
	}
	
	if (wasKeyPressed('w')) {
		if (singlemode) {
			changeMarkerAreaSingle(0, gSin[gCamAngle], -gCos[gCamAngle]);
		} else {
			changeMarkerArea(gSin[gCamAngle], -gCos[gCamAngle]);
		}
  }
	
	if (wasKeyPressed('S'))  {
		changeMarkerAreaSingle(-1, 0, 0);
	}
	
	if (wasKeyPressed('W')) {
		changeMarkerAreaSingle(1, 0, 0)	;
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
	
	if (wasFunctionPressed(4)) {
		markermode = !markermode;
	}

	if (wasFunctionPressed(5)) {
		singlemode = !singlemode;
	}

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

void updateEditor(double interval) {
	Vector3 markerPos;
  
	if (isKeyPressed(KEY_ESC)) {
		saveFieldToFile(gFilename);
		exit(0);
	}
	
	markerPos.x = gCurStart.x + 0.5f;
	markerPos.y = gCurStart.y + 0.5f;
	markerPos.z = SCALE_Z * sgLevel.field[gCurStart.x][gCurStart.y].z;

	updateEditorCamera(interval, markerPos);
	
	if (gIsEditorRunning) {
		animateEditor(interval);
	}
}

void drawEditorField(void) {
	Square square;
	int i, j;
	FieldCoord cur;

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
}

void initEditor(char* filename) {
	static Object oEditor;
	static Object oField;

	glColor3f(1.0f, 1.0f, 1.0f);
	gEditorMainLight = addPointLight(30.0f, 30.0f, 20.0f);

	gFilename = filename;
	loadFieldFromFile(gFilename);

	initObjectGroup(&oEditor);
	sgWindowViewport.world = &oEditor;
	setUpdateFunc(updateEditor);

	initObject(&oField, drawEditorField);
	oField.texture = sgLevel.texture;
	addSubObject(&oEditor, &oField);

	gCurStart.x = 0;
	gCurStart.y = 0;
	gCurEnd.x = 0;
	gCurEnd.y = 0;

	gIsEditorRunning = 1;
}
