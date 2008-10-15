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

#include "tools.hpp"

#include <stdio.h>
#include <string.h>
#include <unistd.h>

void message()
{
	printf("CG Madness, Copyright (C) 2007  Sven Reinck <sreinck@gmail.com>\n");
	printf("CG Madness comes with ABSOLUTELY NO WARRANTY.\n");
	printf("\n");
}

bool assurePath(const char *progname)
{
	const char *lastSlash = NULL;
	bool success = false;

	const char *s;

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
		char *path = new char[length + 1];

		memcpy(path, progname, length);
		path[length] = 0;

		printf("change dir: '%s'\n", path);

		if (chdir(path) == 0)
		{
			success = true;
		}

		delete[] path;
	}

	return success;
}
