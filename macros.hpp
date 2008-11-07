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

#ifndef macros_hpp
#define macros_hpp

#include <cstdio>

#define LENGTH(x) ((int) (sizeof(x) / sizeof(*x)))

#define PRINT_INT(i) printf(#i ": %d\n", (i))
#define PRINT_POINTER(i) printf(#i ": %p\n", (i))
#define PRINT_FLOAT(f) printf(#f ": %f\n", (f))
#define PRINT_VECTOR3(vector) printf(#vector ".x: %f, " #vector ".y: %f, " #vector ".z: %f\n", (vector).x, (vector).y, (vector).z)

#define STRING(x) #x
#define MACRO_STRING(x) STRING(x)

#define FOREACH(list, iter) for (__typeof((list).begin()) iter = (list).begin(); iter != (list).end(); ++iter)

#endif
