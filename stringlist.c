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

#include "stringlist.h"

#include "files.h"

#include "debug.h"

void loadStringList(StringList* list, const char* filename)
{
	char* s;
	int i;
	
	list->all = textFileRead(filename);

	list->count = 0;
	for (s = list->all; *s; s++)
	{
		if (*s == '\n')
		{
			list->count++;
		}
	}
	
	MALLOC(list->strings, sizeof(char*) * list->count);
	
	s = list->all;
	for (i = 0; i < list->count; i++)
	{
		list->strings[i] = s;
		while (*s != '\n')
		{
			s++;
		}
		*s = '\0';
		s++;
	}
}
