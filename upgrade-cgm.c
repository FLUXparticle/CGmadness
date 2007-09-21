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

#include "main.h"
#include "level.h"

#include <stdio.h>
#include <string.h>

void usage(void)
{
	printf("usage: upgrade-cgm <cgm-files...>\n");
}

int main(int argc, char* argv[])
{
	char* file = NULL;
	int i;

	message();

	/* read parameters */
	for (i = 1; i < argc; i++)
	{
		sgLevel.size.x = -1;
		sgLevel.size.y = -1;

		file = argv[i];

		if (loadFieldFromFile(file) && saveFieldToFile(file))
		{
			printf("'%s' upgraded successfully.\n", file);
		} else {
			printf("'%s' not upgraded!\n", file);
		}
	}

	if (!file)
	{
		usage();
		return 1;
	}

  return 0;
}
