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

#include "stringlist.hpp"

#include "files.hpp"

#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <dirent.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static bool isCGM(char *filename)
{
	return strcmp(filename + strlen(filename) - 4, ".cgm") == 0;
}

static const char *findLast(const char *str, char ch)
{
	const char *tmp = str;

	const char *p = NULL;

	while ((tmp = strchr(tmp, ch)))
	{
		p = tmp;
		tmp++;
	}

	return p;
}

void findBasename(const char *path, int *start, int *end)
{
	const char *s;
	const char *e;

	s = findLast(path, '/');

	if (s)
	{
		s++;
	}
	else
	{
		s = path;
	}

	e = findLast(path, '.');

	if (!e)
	{
		e = s + strlen(s);
	}

	*start = s - path;
	*end = e - path;
}

int lengthBasename(const char *path)
{
	int start;
	int end;

	findBasename(path, &start, &end);

	return end - start;
}

char *copyBasename(const char *path, char *dest)
{
	int start;
	int end;
	int len;

	findBasename(path, &start, &end);

	len = end - start;

	memcpy(dest, path + start, len);
	dest[len] = 0;

	return dest + len + 1;
}

void createStringListFromDir(StringList * list, const char *dirname)
{
	struct dirent *fileinfo;
	char *p;
	char **pp;

	int count = 0;
	int size = 0;
	DIR *dirinfo = opendir(dirname);
	int dirnamelen = strlen(dirname);

	if (dirinfo)
	{
		while ((fileinfo = readdir(dirinfo)))
		{
			struct stat filestat;

			char *newdir = new char[dirnamelen + 1 + strlen(fileinfo->d_name) + 1];

			sprintf(newdir, "%s/%s", dirname, fileinfo->d_name);

			if (isCGM(newdir))
			{
				stat(newdir, &filestat);
				if (S_ISREG(filestat.st_mode))
				{
					count++;
					size += strlen(newdir) + 1;
					size += lengthBasename(newdir) + 1;
				}
			}

			delete[] newdir;
		}

		rewinddir(dirinfo);

		list->count = count;
		list->all = new char[size];
		list->strings = new char *[list->count];

		p = list->all;
		pp = list->strings;

		while ((fileinfo = readdir(dirinfo)))
		{
			struct stat filestat;

			char *newdir = new char[dirnamelen + 1 + strlen(fileinfo->d_name) + 1];

			sprintf(newdir, "%s/%s", dirname, fileinfo->d_name);

			if (isCGM(newdir))
			{
				stat(newdir, &filestat);
				if (S_ISREG(filestat.st_mode))
				{
					size = strlen(newdir) + 1;
					memcpy(p, newdir, size);

					*pp = p;

					p += size;

					p = copyBasename(newdir, p);

					pp++;
				}
			}

			delete[] newdir;
		}

		closedir(dirinfo);
	}

	sortStringList(list);
}

void loadStringListFromFile(StringList * list, const char *filename)
{
	char *s;
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

	list->strings = new char *[list->count];

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

static int compare(const void *a, const void *b)
{
	const char **pa = (const char **) a;
	const char **pb = (const char **) b;

	return strcmp(*pa, *pb);
}

void sortStringList(StringList * list)
{
	qsort(list->strings, list->count, sizeof(char *), compare);
}
