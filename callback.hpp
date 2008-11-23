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

#ifndef _callback_hpp_
#define _callback_hpp_

#include "hw/mouse.hpp"

#include "process/Process.hpp"

#include "math/Vector3.hpp"
#include "math/Matrix.hpp"

typedef struct
{
	Matrix projection;
} Viewport;

typedef struct
{
	int width;
	int height;

	int framebuffer;
	Viewport *viewport;
} RenderTarget;

extern Viewport sgWindowViewport;

void centerMouse(int *x, int *y);

void startCallback(Process* process);

void setUpdateFrequency(int callsPerSecond);

#endif
