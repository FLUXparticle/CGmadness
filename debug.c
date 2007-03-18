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

#include "debug.h"

#include <stdio.h>
#include <stdlib.h>

void* dbgMalloc(int size, const char* pointername, const char* filename, int line) {
	void* p = malloc(size);
	if (p) {
		fprintf(stderr, "%s : %d -- malloc %s:%d\n", pointername, size, filename, line);
	} else {
		fprintf(stderr, "%s : %d -- could not malloc %s:%d\n", pointername, size, filename, line);
	}
	return p;
}

void dbgFree(void* p, const char* pointername, const char* filename, int line) {
	fprintf(stderr, "%s -- free %s:%d\n", pointername, filename, line);
	free(p);
}
