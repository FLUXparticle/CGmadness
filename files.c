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

#include "files.h"

#include "debug.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char* textFileRead(const char* fn) {
	FILE* fp;
	char* content = NULL;

	int count = 0;

	if (fn != NULL) {
		fp = fopen(fn, "rt");

		if (fp != NULL) {
      fseek(fp, 0, SEEK_END);
      count = ftell(fp);
      rewind(fp);

			if (count > 0) {
				MALLOC(content, sizeof(char) * (count+1));
				count = fread(content, sizeof(char), count, fp);
				content[count] = '\0';
			}

			fclose(fp);
		}
	}
	return content;
}

int textFileWrite(const char *fn, const char *s) {
	FILE *fp;
	int status = 0;

	if (fn != NULL) {
		fp = fopen(fn,"w");

		if (fp != NULL) {
			if (fwrite(s,sizeof(char),strlen(s),fp) == strlen(s)) {
				status = 1;
			}

			if (fclose(fp) != 0) {
				return 0;
			}
		}
	}
	return status;
}
