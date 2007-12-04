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
 */

#include "tools.h"

#include "debug.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

void message(void)
{
	printf("CG Madness, Copyright (C) 2007  Sven Reinck <sreinck@gmx.de>\n");
	printf("CG Madness comes with ABSOLUTELY NO WARRANTY.\n");
}

char* addStrings(const char* a, const char* b)
{
	char* c;
	
	MALLOC(c, strlen(a) + strlen(b) + 1);
	
	strcpy(c, a);
	strcat(c, b);
	
	return c;
}

void assurePath(const char* progname)
{
	const char* lastSlash = NULL;

	const char* s;

	for (s = progname; *s; s++)
	{
		if (*s == '/')
		{
			lastSlash = s;
		}
	}

	if (lastSlash)
	{
		int length = lastSlash - progname;
		char* path;

		MALLOC(path, length + 1);

		if (path)
		{
			memcpy(path, progname, length);
			path[length] = 0;

			printf("change dir: '%s'\n", path);

			chdir(path);

			FREE(path);
		}
	}
}
