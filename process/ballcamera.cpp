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

#include "ballcamera.hpp"

#include "hw/features.hpp"
#include "hw/keyboard.hpp"
#include "camera.hpp"

#include "functions.hpp"

#include <cmath>

static bool gIsMouseControl = false;

static Vector3 gForward;
static Vector3 gRight;

static bool gIsBallActive = false;
static bool gIsBallControlActive = false;
static bool gIsBallCameraActive = false;

static float gDistance;
static float gLatitude;
static float gLongitude;

static int gDragX = 0;
static int gDragY = 0;

void gameDrag(int dx, int dy)
{
	gDragX += dx;
	gDragY += dy;
}

void resetBallCamera(void)
{
	gDistance = 5.0f;
	gLatitude = 20.0f;
	gLongitude = 0.0f;
}

void enableBall()
{
	gIsBallActive = true;
	gIsBallControlActive = true;
}

void disableBall()
{
	gIsBallActive = false;
}

void enableBallCamera()
{
	if (gIsMouseControl)
	{
		setDragFunc(gameDrag);
	}

	gIsBallCameraActive = true;
}

void disableBallCamera()
{
	setDragFunc(NULL);

	gIsBallCameraActive = false;
}

void updateBallCamera(float interval, Vector3 ball)
{
	Vector3 diff;
	Vector3 up(0.0f, 0.0f, 1.0f);
	static Vector3 dest(0.0f, 0.0f, 0.0f);

	/* game controls for camera */

	if (gIsMouseControl)
	{
		gLongitude -= 5.0f * interval * gDragX;
		gLatitude += 5.0f * interval * gDragY;

		gDragX = 0;
		gDragY = 0;
	}
	else
	{
		/* zoom */
		if (isKeyPressed('f') && gDistance < 20.0f)
			gDistance += 0.1f;
		if (isKeyPressed('r') && gDistance > 0.5)
			gDistance -= 0.1f;

		/* rotation */
		if (isKeyPressed('a'))
			gLongitude -= 120.0f * interval;
		if (isKeyPressed('d'))
			gLongitude += 120.0f * interval;

		/* height */
		if (isKeyPressed('w'))
			gLatitude += 120.0f * interval;
		if (isKeyPressed('s'))
			gLatitude -= 120.0f * interval;

		gLatitude = clamp(gLatitude, -89.0f, 89.0f);
	}

	dest.x =
		ball.x +
		gDistance * sin(gLongitude * M_PI / 180.0f) * cos(gLatitude * M_PI / 180.0f);
	dest.y =
		ball.y -
		gDistance * cos(gLongitude * M_PI / 180.0f) * cos(gLatitude * M_PI / 180.0f);
	dest.z = ball.z + gDistance * sin(gLatitude * M_PI / 180.0f);

	moveCamera(interval, dest, ball);

	if (gIsMouseControl)
	{
		sgCamera = dest;
	}

	diff = sub(sgLookat, sgCamera);
	diff.z = 0.0f;
	gForward = norm(diff);
	gRight = norm(cross(gForward, up));
}

void toggleMouseControl()
{
	gIsMouseControl = !gIsMouseControl;
}

void updateBall(Ball& ball, float interval)
{
	Vector3 force(0.0f, 0.0f, 0.0f);

	if (gIsBallControlActive)
	{
		/* ball controls */
		if (gIsMouseControl)
		{
			if (isKeyPressed('a'))
			{
				force = sub(force, gRight);
			}

			if (isKeyPressed('d'))
			{
				force = add(force, gRight);
			}

			if (isKeyPressed('s'))
			{
				force = sub(force, gForward);
			}

			if (isKeyPressed('w'))
			{
				force = add(force, gForward);
			}
		}
		else
		{
			if (isCursorPressed(CURSOR_LEFT))
			{
				force = sub(force, gRight);
			}

			if (isCursorPressed(CURSOR_RIGHT))
			{
				force = add(force, gRight);
			}

			if (isCursorPressed(CURSOR_DOWN))
			{
				force = sub(force, gForward);
			}

			if (isCursorPressed(CURSOR_UP))
			{
				force = add(force, gForward);
			}
		}
	}

	if (gIsBallActive)
	{
		ball.push(force);
		if (isKeyPressed(' '))
		{
			ball.jump();
		}

		ball.update(interval);
	}

	if (gIsBallCameraActive)
	{
		updateBallCamera(interval, ball.pos());
	}
}
