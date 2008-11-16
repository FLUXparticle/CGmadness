/*
 * CG Madness - a Marble Madness clone
 * Copyright (C) 2007  Sven Reinck <sreinck@gmail.com>
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

#include "Editor.hpp"

#include "screen/editor/ScreenEditorMain.hpp"
#include "screen/ScreenWait.hpp"
#include "screen/ScreenInfo.hpp"

#include "kdtree/KdRangeTraverse.hpp"

#include "utils/Callback.hpp"

#include "level/io.hpp"
#include "level/level.hpp"

#include "ballcamera.hpp"
#include "field.hpp"
#include "camera.hpp"
#include "hw/keyboard.hpp"
#include "common.hpp"

#include "Color.hpp"

#include "macros.hpp"

#include GLUT_H

#include <cstdlib>
#include <cmath>

static FieldCoord gCurStart;
static FieldCoord gCurEnd;
static int gCamAngle = 0;

static const int gSin[] = { 0, 1, 0, -1 };
static const int gCos[] = { 1, 0, -1, 0 };

static bool gDirtyLightmaps;

Editor::Editor()
{
	gScreenEditorMain = new ScreenEditorMain(this);
}

Editor::~Editor()
{
	// empty
}

void Editor::start(Process* previous)
{
	mPrevious = previous;

	sgLevel.lightMap = 0;
	if (sgLevel.saved)
	{
		gDirtyLightmaps = false;
	}

	gCurStart.x = 0;
	gCurStart.y = 0;
	gCurEnd.x = 0;
	gCurEnd.y = 0;

	mState = STATE_PAUSED;

	Main::pushState(gScreenEditorMain);
}

void Editor::suspend()
{
	mState = STATE_PAUSED;
}

void Editor::resume()
{
	glutSetCursor(GLUT_CURSOR_NONE);
	mState = STATE_EDITING;
}

void Editor::lightMapsReady()
{
	gDirtyLightmaps = false;
	saveLevel();
}

void Editor::saveLevel()
{
	if (gDirtyLightmaps)
	{
		destroyCommon();

		gScreenWait = new ScreenWait( METHOD_CALLBACK(Editor, lightMapsReady) );

		initCommon();
		setUpdateFrequency(10);
		Main::setState(gScreenWait, false);
		updateLightMap(true);
	}
	else
	{
		if (saveLevelToFile())
		{
			sgLevel.saved = true;
			gScreenInfo = new ScreenInfo("level saved successfully");
			Main::setState(gScreenInfo, false);
		}
		else
		{
			gScreenInfo = new ScreenInfo("operation failed");
			Main::setState(gScreenInfo, false);
		}
	}
}

void updateEditorCamera(float interval, Vector3 marker)
{
	static float distance = 5.0f;
	static float height = 2.0f;
	static Vector3 dest(0.0f, 0.0f, 0.0f);
	float angle;

	/* camera controls for editor */

	/* zoom */
	if (isKeyPressed('v'))
		distance += 0.1f;
	if (isKeyPressed('c') && distance > 0.5)
		distance -= 0.1f;

	/* rotation */
	if (wasKeyPressed('b'))
		gCamAngle--;
	if (wasKeyPressed('n'))
		gCamAngle++;

	if (gCamAngle < 0)
		gCamAngle += 4;
	if (gCamAngle >= 4)
		gCamAngle -= 4;

	/* height */
	if (isKeyPressed('x'))
		height += 0.1f;
	if (isKeyPressed('y'))
		height -= 0.1f;

	angle = gCamAngle * 90.0f;

	/* new camera position */
	dest.x = sin(angle * M_PI / 180.0f) * distance + marker.x;
	dest.y = -cos(angle * M_PI / 180.0f) * distance + marker.y;
	dest.z = height + marker.z;

	moveCamera(interval, dest, marker);
}

static void markerChanged()
{
	Vector3 min(gCurStart.x - 0.5f, gCurStart.y - 0.5f, 0.0f);
	Vector3 max(gCurEnd.x + 1.5f, gCurEnd.y + 1.5f, 0.0f);
	KdRangeTraverse iter(*sgLevel.kdLevelTree, min, max);

	while (iter.next())
	{
		Block& b = sgLevel.blocks[(*iter).start];
		b.dirty = true;
	}

	sgLevel.saved = false;
	gDirtyLightmaps = true;
}

void changeMarkerArea(int incz, int incdzx, int incdzy)
{
	Vector3 min(gCurStart.x + 0.5f, gCurStart.y + 0.5f, 0.0f);
	Vector3 max(gCurEnd.x + 0.5f, gCurEnd.y + 0.5f, 0.0f);
	KdRangeTraverse iter(*sgLevel.kdLevelTree, min, max);

	while (iter.next())
	{
		Block& b = sgLevel.blocks[(*iter).start];
		int x = b.x;
		int y = b.y;
		int z = b.z;
		int dzx = b.dzx;
		int dzy = b.dzy;

		int incx = -(gCurEnd.x + gCurStart.x - 2 * x) * incdzx;
		int incy = -(gCurEnd.y + gCurStart.y - 2 * y) * incdzy;

		z += incz + incx + incy;
		dzx += incdzx;
		dzy += incdzy;

		if (z - (abs(dzx) + abs(dzy)) >= 0 && (z + (abs(dzx) + abs(dzy)))
				<= MAX_LEVEL_HEIGHT * HEIGHT_STEPS && abs(dzx) <= 5 && abs(dzy) <= 5)
		{
			b.z = z;
			b.dzx = dzx;
			b.dzy = dzy;
		}
	}

	markerChanged();
}

static void flattenMarkerArea()
{
	Vector3 min(gCurStart.x + 0.5f, gCurStart.y + 0.5f, 0.0f);
	Vector3 max(gCurEnd.x + 0.5f, gCurEnd.y + 0.5f, 0.0f);
	KdRangeTraverse iter(*sgLevel.kdLevelTree, min, max);

	while (iter.next())
	{
		Block& b = sgLevel.blocks[(*iter).start];

		b.dzx = 0;
		b.dzy = 0;
	}

	markerChanged();
}

void modBetween(int& value, int mod, int min, int max)
{
	value += mod;

	if (value < min)
	{
		value = min;
	}
	else if (value >= max)
	{
		value = max - 1;
	}
}

void moveMarker(int markermode, int dx, int dy)
{
	if (markermode)
	{
		modBetween(gCurEnd.x, dx, gCurStart.x, sgLevel.size.x);
		modBetween(gCurEnd.y, dy, gCurStart.y, sgLevel.size.y);
	}
	else
	{
		int sx = gCurEnd.x - gCurStart.x;
		int sy = gCurEnd.y - gCurStart.y;

		modBetween(gCurStart.x, dx, 0, sgLevel.size.x - sx);
		modBetween(gCurStart.y, dy, 0, sgLevel.size.y - sy);

		gCurEnd.x = gCurStart.x + sx;
		gCurEnd.y = gCurStart.y + sy;
	}
}

void moveMarkerLeft(int markermode)
{
	moveMarker(markermode, -gCos[gCamAngle], -gSin[gCamAngle]);
}

void moveMarkerRight(int markermode)
{
	moveMarker(markermode, gCos[gCamAngle], gSin[gCamAngle]);
}

void moveMarkerUp(int markermode)
{
	moveMarker(markermode, -gSin[gCamAngle], gCos[gCamAngle]);
}

void moveMarkerDown(int markermode)
{
	moveMarker(markermode, gSin[gCamAngle], -gCos[gCamAngle]);
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
		changeMarkerArea(1, 0, 0);
	}

	if (wasKeyPressed('0'))
	{
		flattenMarkerArea();
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

void Editor::enableTestMode()
{
	mBall.reset();

	resetBallCamera();
	enableBallCamera();

	initGameField();

	mState = STATE_TESTING;
}

void Editor::disableTestMode()
{
	destroyGameField();
	disableBallCamera();

	mState = STATE_EDITING;
}

void Editor::update(float interval)
{
	switch (mState)
	{
	case STATE_TESTING:
		if (wasKeyPressed(KEY_ESC))
		{
			disableTestMode();
		}

		updateBall(mBall, interval);
		break;
	case STATE_EDITING:
	{
		Vector3 markerPos;

		if (wasKeyPressed(KEY_ESC))
		{
			Main::pushState(gScreenEditorMain);
		}

		if (wasKeyPressed(KEY_ENTER))
		{
			enableTestMode();
		}

		KdCell::Range range = sgLevel.kdLevelTree->get(gCurStart.x, gCurStart.y);

		markerPos.x = (gCurStart.x + gCurEnd.x) / 2.0f + 0.5f;
		markerPos.y = (gCurStart.y + gCurEnd.y) / 2.0f + 0.5f;
		markerPos.z = (float) sgLevel.blocks[range.start].z / HEIGHT_STEPS;

		updateEditorCamera(interval, add(markerPos, sgLevel.origin));
		animateEditor(interval);
		break;
	}
	default:
		break;
	}
}

void drawEditorField(bool showCursor)
{
	FieldCoord cur;

	float pos[4] = { 0.0f, 0.0f, 1.0f, 0.0f };

	glLightfv(GL_LIGHT0, GL_POSITION, pos);

	glEnable(GL_LIGHTING);
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, sgLevel.borderTexture);

	for (cur.x = 0; cur.x < sgLevel.size.x; cur.x++)
	{
		for (cur.y = 0; cur.y < sgLevel.size.y; cur.y++)
		{
			if (showCursor && cur.x >= gCurStart.x && cur.x <= gCurEnd.x && cur.y
					<= gCurEnd.y && cur.y >= gCurStart.y)
			{
				glColor3fv(Color4::red);
			}
			else if (cur.x == sgLevel.start.x && cur.y == sgLevel.start.y)
			{
				glColor3fv(Color4::green);
			}
			else if (cur.x == sgLevel.finish.x && cur.y == sgLevel.finish.y)
			{
				glColor3fv(Color4::blue);
			}
			else
			{
				glColor3fv(Color4::white);
			}

			const Square& square = getRoofSquare(cur.x, cur.y);

			glBegin(GL_QUADS);
			glNormal3fv(square.normal);
			for (int i = 0; i < 4; i++)
			{
				glTexCoord2fv(square.texCoords(i));
				glVertex3fv(square.vertices[i]);
			}
			glEnd();

			glColor3f(1.0f, 1.0f, 1.0f);

			glBegin(GL_QUADS);
			for (int j = 0; j < 4; j++)
			{
				const SideFace& face = getSideFace(cur.x, cur.y, j);

				FOREACH(face.squares, iter)
				{
					glNormal3fv(iter->normal);
					for (int i = 0; i < 4; i++)
					{
						glTexCoord2fv(iter->texCoords(i));
						glVertex3fv(iter->vertices[i]);
					}
				}
			}
			glEnd();
		}
	}

	glDisable(GL_TEXTURE_2D);
	glDisable(GL_LIGHTING);
}

void Editor::draw() const
{
	drawEditorField(mState == STATE_EDITING);

	if (mState == STATE_TESTING)
	{
		mBall.drawGameBall();
	}
}
