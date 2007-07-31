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

#ifndef _callback_h_
#define _callback_h_

#include "graph.h"
#include "mouse.h"

#include "types.h"

typedef void (*funcUpdate)(float interval);
typedef void (*funcDrawHUD)(float width, float height);

typedef void (*funcDoMouseEvent)(const Vector3* position, const Vector3* direction, MouseEvent event);

typedef struct {
	Matrix projection;
	Matrix view;
	funcDraw draw;
	funcDrawHUD drawHUD;
	funcDoMouseEvent mouseEvent;
} Viewport;

typedef struct {
	int width;
	int height;

	int framebuffer;
	Viewport* viewport;
} RenderTarget;

extern Viewport sgWindowViewport;

void setUpdateFunc(funcUpdate update);
void setPreDisplayFunc(funcDraw preDisplay);

void centerMouse(int* x, int* y);

void startDisplay(void);

void startTimer(int callsPerSecond);

void mouseEvent(int x, int y, MouseEvent event);

#endif
