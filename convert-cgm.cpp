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

#include "level/types.hpp"
#include "level/io.hpp"

#include "tools.hpp"

#include "utils/String.hpp"

#include <cstdio>
#include <cstdlib>
#include <cstring>

void usage()
{
	printf("usage: convert-cgm [parameters...] <cgm-file>\n");
	printf("\n");
	printf("  if <cgm-file> does not exits, it will be created but only if a size is given\n");
	printf("  --size x y      size of the new created level\n");
	printf("  --author name   change author of this level\n");
	printf("  --shrink        shrink level to the minimum needed size\n");
	printf("\n");
}

int main(int argc, char *argv[])
{
	message();

	bool setAuthor = false;
	bool doShrink = false;
	String author;
	const char* file = NULL;

	sgLevel.size.x = -1;
	sgLevel.size.y = -1;

	/* read parameters */
	for (int i = 1; i < argc; i++)
	{
		if (strcmp(argv[i], "--size") == 0 && i + 2 < argc)
		{
			sgLevel.size.x = atoi(argv[++i]);
			sgLevel.size.y = atoi(argv[++i]);
		}
		else if (strcmp(argv[i], "--author") == 0 && i + 1 < argc)
		{
			author = argv[++i];
			setAuthor = true;
		}
		else if (strcmp(argv[i], "--shrink") == 0)
		{
			doShrink = true;
		}
		else
		{
			file = argv[i];
		}
	}

	if (!file)
	{
		usage();
		return 1;
	}

	if (loadLevelFromFile(file))
	{
		if (setAuthor)
		{
			strncpy(sgLevel.author, author, MAX_NAME_LENGTH);
			sgLevel.author[MAX_NAME_LENGTH] = '\0';
		}

		if (saveLevelToFile(doShrink))
		{
			printf("'%s' processed successfully.\n", file);
		}
	}
	else
	{
		printf("'%s' not processed!\n", file);
	}

	return 0;
}
