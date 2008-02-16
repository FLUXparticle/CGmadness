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

#ifndef _types_hpp_
#define _types_hpp_

/*
 * namen conventions for variables:
 * member                 m
 * static member          s
 * global                 g
 * super global           sg
 */

#define PI 3.14159265358979323846

#define LENGTH(x) ((int) (sizeof(x) / sizeof(*x)))

#define FOV 60.0f

#define MATRIX_SIZE 4

typedef float Matrix[MATRIX_SIZE][MATRIX_SIZE];

typedef void (*funcUpdate) (float interval);
typedef void (*funcDraw) (void);

#endif
