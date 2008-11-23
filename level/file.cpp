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

#include "file.hpp"

#include "crc32.hpp"

#include "atlas.hpp"

int readByte(FILE* file)
{
	int value = -1;

	if (fscanf(file, "%i", &value) == 1)
	{
		nextByte(value);
	}

	return value;
}

int readInt(FILE* file)
{
	int value = -1;

	if (fscanf(file, "%i", &value) == 1)
	{
		int tmp = value;

		for (int i = 0; i < 4; i++)
		{
			nextByte(tmp);
			tmp >>= 8;
		}
	}

	return value;
}

void readString(FILE* file, char* str)
{
	char *s = str;

	fgetc(file);

	for (int i = 0; i < MAX_NAME_LENGTH; i++)
	{
		int b = fgetc(file);

		if (b < MIN_ALLOWED_CHAR)
		{
			break;
		}

		*s = b;
		nextByte(b);

		s++;
	}

	*s = '\0';
}

static void readRLEByte(FILE* file, int& repeat, int& value)
{
	if (fscanf(file, "%ix%i", &repeat, &value) < 2)
	{
		value = repeat;
		repeat = 1;
	}

	for (int i = 0; i < repeat; i++)
	{
		nextByte(value);
	}
}

void readRLE(FILE* file, int data[SIZEOF_LIGHT_MAP])
{
	int s = 0;

	while (s < SIZEOF_LIGHT_MAP)
	{
		int repeat;
		int value;

		readRLEByte(file, repeat, value);

		for (int i = 0; i < repeat; i++)
		{
			data[s + i] = value;
		}

		s += repeat;
	}
}

void readFieldCoord(FILE* file, FieldCoord& coord)
{
	coord.x = readByte(file);
	coord.y = readByte(file);
}

void readFieldBlockV3(FILE* file, Block& block)
{
	block.z = readByte(file);
	block.dzx = readByte(file);
	block.dzy = readByte(file);
}

void readFieldBlock(FILE* file, Block& block)
{
	block.x = readByte(file);
	block.y = readByte(file);
	block.z = readByte(file);
	block.dzx = readByte(file);
	block.dzy = readByte(file);
}

void writeByte(FILE* file, int value)
{
	fprintf(file, "%i", value);
	nextByte(value);
}

void writeInt(FILE* file, int value)
{
	int i;

	fprintf(file, "%i", value);

	for (i = 0; i < 4; i++)
	{
		nextByte(value);
		value >>= 8;
	}
}

void writeString(FILE* file, char* value)
{
	const char *s;

	for (s = value; *s; s++)
	{
		fputc(*s, file);
		nextByte(*s);
	}
}

void writeFieldCoord(FILE* file, const FieldCoord& coord)
{
	writeByte(file, coord.x);
	fputc(' ', file);
	writeByte(file, coord.y);
	fputc('\n', file);
}

void writeFieldBlock(FILE* file, const Block& block)
{
	writeByte(file, block.x);
	fputc(' ', file);
	writeByte(file, block.y);
	fputc(' ', file);
	writeByte(file, block.z);
	fputc(' ', file);
	writeByte(file, block.dzx);
	fputc(' ', file);
	writeByte(file, block.dzy);
	fputc('\n', file);
}

static void writeRLEInt(FILE* file, int repeat, int value)
{
	int i;

	if (repeat > 1)
	{
		fprintf(file, "%ix%i", repeat, value);
	}
	else
	{
		fprintf(file, "%i", value);
	}

	for (i = 0; i < repeat; i++)
	{
		nextByte(value);
	}
}

void writeRLE(FILE* file, const int data[SIZEOF_LIGHT_MAP])
{
	int s = 0;
	int l = 1;

	while (s < SIZEOF_LIGHT_MAP)
	{
		if (s + l < SIZEOF_LIGHT_MAP && data[s] == data[s + l])
		{
			l++;
		}
		else
		{
			if (s > 0)
			{
				fputc(' ', file);
			}
			writeRLEInt(file, l, data[s]);

			s += l;
			l = 1;
		}
	}
	fputc('\n', file);
}
