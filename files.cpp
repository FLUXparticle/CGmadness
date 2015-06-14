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

#include "files.hpp"

#include <stdio.h>
#include <string>

char *textFileRead(const char *fn)
{
	FILE *fp;
	char *content = NULL;

	std::string filename = "shaders/";
	filename += fn;

	long count = 0;

	fp = fopen(filename.c_str(), "rt");

	if (fp != NULL) {
		fseek(fp, 0, SEEK_END);
		count = ftell(fp);
		rewind(fp);

		if (count > 0) {
			content = new char[count + 1];
			count = fread(content, sizeof(char), count, fp);
			content[count] = '\0';
		}

		fclose(fp);
	}

	return content;
}

