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
 *
 * $Id$
 *
 */

#ifndef _callback_h_
#define _callback_h_

#include "graph.h"

#include "types.h"

typedef void (*funcUpdate)(float interval);
/*
 * typedef void (*funcDraw)(void);
 */

typedef void (*funcDoPick)(void);

typedef struct {
	Matrix projection;
	Matrix view;
	funcDraw draw;
	funcDoPick pick;
} Viewport;

typedef struct {
	int enabled;

	int width;
	int height;

	int framebuffer;
	int texTarget;
	int texID;
	Viewport* viewport;
} RenderTarget;

extern Viewport sgWindowViewport;

void setUpdateFunc(funcUpdate update);
void setPreDisplayFunc(funcDraw preDisplay);
	
void centerMouse(int* x, int* y);

void startDisplay(void);

void startTimer(int callsPerSecond);

int pick(int x, int y); 

#endif
