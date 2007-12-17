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

#ifndef _debug_hpp_
#define _debug_hpp_

#define DEBUG_TIME 0

#define GL_DEBUG(x) { int error, before = glGetError(); x; error = glGetError(); if (before || error) printf("0x%x -> %s: 0x%x\n", before, #x, error); }

#if DEBUG_TIME
#  define TIME(x) { int after, before = glutGet(GLUT_ELAPSED_TIME); x; after = glutGet(GLUT_ELAPSED_TIME); if (after >= before + 10) printf("%s: %d ms\n", #x, after - before); }
#else
#  define TIME(x) x
#endif

#endif
