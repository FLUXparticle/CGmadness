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

#include "editor.h"

#include "editormenu.h"

#include "level.h"
#include "callback.h"
#include "camera.h"
#include "keyboard.h"
#include "texture.h"
#include "atlas.h"
#include "common.h"

#include "functions.h"

#include "types.h"
#include "debug.h"

#include <GL/glew.h>
#include <GL/glut.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <float.h>

#define MAX_LEVEL_SIZE 100

#define DRAW_DEBUG_LINES 0

typedef struct {
	Vector3 v1;
	Vector3 v2;
} Line;

#if (DRAW_DEBUG_LINES)
Line gLines[10 * 10 * LIGHT_MAP_SIZE * LIGHT_MAP_SIZE * 32 * 8];
int gCntLines = 0;
#endif

static int gIsEditorRunning;

static FieldCoord gCurStart;
static FieldCoord gCurEnd;
static int gCamAngle = 0;

static int gSin[] = { 0, 1, 0, -1 };
static int gCos[] = { 1, 0, -1, 0 };

static char* gFilename;

static Vector3 gEditorScreenPosition;

static int gDirtyTexCoords;
static int gDirtyLightmaps;

void pauseEditor(void) {
	showEditorScreen(0);
	gIsEditorRunning = 0;
}

void resumeEditor(void) {
	glutSetCursor(GLUT_CURSOR_NONE);
	gIsEditorRunning = 1;
}

void saveLevel(void) {
	if (gDirtyLightmaps)
	{
		destroyCommon();

		initCommon();
		updateLightMap();
		gDirtyLightmaps = 0;
	}

	if (saveFieldToFile(gFilename)) {
		showEditorScreen(1);
	} else {
		showEditorScreen(2);
	}
}

#if (DRAW_DEBUG_LINES)
void addLine(Vector3 v1, Vector3 v2) {
	gLines[gCntLines].v1 = v1;
	gLines[gCntLines].v2 = v2;
	gCntLines++;
}
#endif

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

			if (z - (abs(dzx) + abs(dzy)) >= 0 && (z + (abs(dzx) + abs(dzy))) <= MAX_LEVEL_HEIGHT * HEIGHT_STEPS && abs(dzx) <= 5 && abs(dzy) <= 5)  {
				p->z = z;
				p->dzx = dzx;
				p->dzy = dzy;
			}

			incy += 2 * incdzy;
		}

		incx += 2 * incdzx;
	}

	for (x = gCurStart.x - 1; x <= gCurEnd.x + 1; x++)
	{
		for (y = gCurStart.y - 1; y <= gCurEnd.y + 1; y++)
		{
			sgLevel.field[x][y].dirty = 1;
		}
	}
	gDirtyLightmaps = 1;
	gDirtyTexCoords = 1;
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

void moveMarkerDown(int markermode)
{
	moveMarker(markermode,  gSin[gCamAngle], -gCos[gCamAngle]);
}

void animateEditor(float interval)
{
	int markermode = 0;

	/* editor controls for changing environment */
	if (wasKeyPressed('a'))
	{
		changeMarkerArea(0, gCos[gCamAngle], gSin[gCamAngle]);
	}

	if (wasKeyPressed('d'))
	{
		changeMarkerArea(0, -gCos[gCamAngle], -gSin[gCamAngle]);
	}


	if (wasKeyPressed('s'))
	{
		changeMarkerArea(0, -gSin[gCamAngle], gCos[gCamAngle]);
	}

	if (wasKeyPressed('w'))
	{
		changeMarkerArea(0, gSin[gCamAngle], -gCos[gCamAngle]);
  }

	if (wasKeyPressed('f'))
	{
		changeMarkerArea(-1, 0, 0);
	}

	if (wasKeyPressed('r'))
	{
		changeMarkerArea(1, 0, 0)	;
	}

	/* editor controls for current field */
	if (wasKeyPressed('1'))
	{
		if (gCurStart.x != sgLevel.finish.x || gCurStart.y != sgLevel.finish.y)
		{
			sgLevel.start.x = gCurStart.x;
			sgLevel.start.y = gCurStart.y;
		}
	}

	if (wasKeyPressed('2'))
	{
		if (gCurStart.x != sgLevel.start.x || gCurStart.y != sgLevel.start.y)
		{
			sgLevel.finish.x = gCurStart.x;
			sgLevel.finish.y = gCurStart.y;
	  }
	}

	markermode = getModifiers() == GLUT_ACTIVE_SHIFT;

	/*  editor controls for moving selection */
	if (wasCursorPressed(CURSOR_LEFT))
	{
		moveMarkerLeft(markermode);
	}

	if (wasCursorPressed(CURSOR_RIGHT))
	{
		moveMarkerRight(markermode);
	}

	if (wasCursorPressed(CURSOR_DOWN))
	{
		moveMarkerDown(markermode);
	}

	if (wasCursorPressed(CURSOR_UP))
	{
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
		markerPos.z = (float) sgLevel.field[gCurStart.x][gCurStart.y].z / HEIGHT_STEPS;

		updateEditorCamera(interval, markerPos);
		animateEditor(interval);
		
		if (gDirtyTexCoords)
		{
			updateTexCoords();
			gDirtyTexCoords = 0;
		}
	}
	else
	{
		/* show menu */
		Vector3 camera = gEditorScreenPosition;
		Vector3 lookat = gEditorScreenPosition;

		camera.y -= 10.0f;
		camera.z += 7.0f;

		lookat.z += 5.0f;

		updateEditorScreen(interval);

		moveCamera(interval, camera, lookat);
	}
}

void drawEditorField(void) {
	int i;
	int j;
	FieldCoord cur;
	Square square;

	glEnable(GL_LIGHTING);
	glEnable(GL_COLOR_MATERIAL);
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, sgLevel.borderTexture);

		for (cur.x = 0; cur.x < sgLevel.size.x; cur.x++)
		{
			for (cur.y = 0; cur.y < sgLevel.size.y; cur.y++)
			{

				if (cur.x >= gCurStart.x && cur.x <= gCurEnd.x && cur.y <= gCurEnd.y && cur.y >= gCurStart.y)
				{
					glColor3f(1.0f, 0.0f, 0.0f);
				}
				else if (cur.x == sgLevel.start.x && cur.y == sgLevel.start.y)
				{
					glColor3f(0.0f, 1.0f, 0.0f);
				}
				else if (cur.x == sgLevel.finish.x && cur.y == sgLevel.finish.y)
				{
					glColor3f(0.0f, 0.0f, 1.0f);
				}
				else
				{
					glColor3f(1.0f, 1.0f, 1.0f);
				}

				getRoofSquare(cur.x, cur.y, &square);

				glBegin(GL_QUADS);
					glNormal3fv(&square.normal.x);
					for (i = 0; i < 4; i++) {
						glTexCoord2fv(&square.texcoord[i].x);
						glVertex3fv(&square.vertices[i].x);
					}
				glEnd();

				glColor3f(1.0f, 1.0f, 1.0f);

				glBegin(GL_QUADS);
					for (j = 0; j < 4; j++) {
						SideFace face;
						int k;

						getSideFace(cur.x, cur.y, j, &face);

						for (k = 0; k < face.cntSquares; k++) {
							glNormal3fv(&face.squares[k].normal.x);
							for (i = 0; i < 4; i++) {
								glTexCoord2fv(&face.squares[k].texcoord[i].x);
								glVertex3fv(&face.squares[k].vertices[i].x);
							}
						}
					}
				glEnd();
			}
		}

	glDisable(GL_TEXTURE_2D);
	glDisable(GL_COLOR_MATERIAL);
	glDisable(GL_LIGHTING);

#if (DRAW_DEBUG_LINES)
	glBegin(GL_LINES);
	for (i = 0; i < gCntLines; i++)
	{
		glColor3f(1.0f, 0.0f, 0.0f);
		glVertex3f(gLines[i].v1.x, gLines[i].v1.y, gLines[i].v1.z);
		glColor3f(0.0f, 1.0f, 0.0f);
		glVertex3f(gLines[i].v2.x, gLines[i].v2.y, gLines[i].v2.z);
	}
	glEnd();
#endif
}

void drawEditor(void) {
	drawEditorField();

	if (!gIsEditorRunning)	{
		drawEditorScreen();
	}
}

void eventEditor(const Vector3* position, const Vector3* direction, MouseEvent event)
{
	if (!gIsEditorRunning)
	{
		/*
		 * TODO pass event to menu
		 */
	}
}

int initEditor(char* filename)
{
	gFilename = filename;
	if (!loadFieldFromFile(gFilename))
	{
		if (between(sgLevel.size.x, 1, MAX_LEVEL_SIZE) && between(sgLevel.size.y, 1, MAX_LEVEL_SIZE))
		{
			newLevel();
		}
		else
		{
			return 0;
		}
	}

	if (sgLevel.borderTexture == 0)
	{
		sgLevel.borderTexture = loadTexture("data/plate.tga", 1);
	}

	sgLevel.lightMap = 0;
	gDirtyLightmaps = 0;

	updateTexCoords();
	gDirtyTexCoords = 0;

	sgWindowViewport.drawHUD = NULL;
	sgWindowViewport.mouseEvent = eventEditor;

	gCurStart.x = 0;
	gCurStart.y = 0;
	gCurEnd.x = 0;
	gCurEnd.y = 0;

	initEditorScreen();

	gEditorScreenPosition.x = sgLevel.size.x / 2.0f;
	gEditorScreenPosition.y = -10.0f;
	gEditorScreenPosition.z =   0.0f;

	setEditorScreenPosition(gEditorScreenPosition);

	pauseEditor();

	return 1;
}
