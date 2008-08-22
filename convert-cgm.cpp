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

#include "level.hpp"

#include "tools.hpp"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void usage(void)
{
	printf("usage: upgrade-cgm [parameters...] <cgm-files...>\n");
	printf
		("  --size x y  resize all levels after this parameter to given size\n");
	printf("\n");
	printf
		("  if <cgm-file> does not exits, it will be created but only if a size is given\n");
	printf("\n");
}

int main(int argc, char *argv[])
{
	char *file = NULL;
	int i;

	message();

	sgLevel.size.x = -1;
	sgLevel.size.y = -1;

	/* read parameters */
	for (i = 1; i < argc; i++)
	{
		if (strcmp(argv[i], "--size") == 0 && i + 2 < argc)
		{
			i++;
			sgLevel.size.x = atoi(argv[i++]);
			sgLevel.size.y = atoi(argv[i++]);
		}

		file = argv[i];

		if (loadLevelFromFile(file, 0) && saveLevelToFile())
		{
			printf("'%s' processed successfully.\n", file);
		}
		else
		{
			printf("'%s' not processed!\n", file);
		}
	}

	if (!file)
	{
		usage();
		return 1;
	}

	return 0;
}
