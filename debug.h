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

#ifndef _debug_h_
#define _debug_h_

#define DEBUG_MEMORY 0
#define DEBUG_TIME 0

#define PRINT_INT(i) printf(#i ": %d\n", (i))
#define PRINT_FLOAT(f) printf(#f ": %f\n", (f))
#define PRINT_VECTOR3(vector) printf(#vector ".x: %f, " #vector ".y: %f, " #vector ".z: %f\n", (vector).x, (vector).y, (vector).z)

#define GL_DEBUG(x) { int error, before = glGetError(); x; error = glGetError(); if (before || error) printf("0x%x -> %s: 0x%x\n", before, #x, error); }

#if DEBUG_TIME
#  define TIME(x) { int after, before = glutGet(GLUT_ELAPSED_TIME); x; after = glutGet(GLUT_ELAPSED_TIME); if (after >= before + 10) printf("%s: %d ms\n", #x, after - before); }
#else
#  define TIME(x) x
#endif

#if DEBUG_MEMORY

#  define MALLOC(p, size) p = dbgMalloc(size, #p, __FILE__, __LINE__)
#  define FREE(p) dbgFree(p, #p, __FILE__, __LINE__)

void* dbgMalloc(int size, const char* pointername, const char* filename, int line);
void dbgFree(void* p, const char* pointername, const char* filename, int line);

#else

#  define MALLOC(p, size) p = saveMalloc(size, #p, __FILE__, __LINE__)
#  define FREE(p) free(p)

void* saveMalloc(int size, const char* pointername, const char* filename, int line);

#endif

#endif
