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

#include "Image.hpp"

struct TGAHeader
{
	GLubyte lenID;
	GLubyte typePalette;
	GLubyte typeImage;
	GLushort startPalette;
	GLushort lenPalette;
	GLubyte sizePaletteEntry;
	GLushort startX;
	GLushort startY;
	GLushort width;
	GLushort height;
	GLubyte bitsPerPixel;
	GLubyte attrImage;
};

void copyPixel(GLubyte * data, int pos, GLubyte * pixel, int components)
{
	data[pos++] = pixel[2];
	data[pos++] = pixel[1];
	data[pos++] = pixel[0];
	if (components > 3)
	{
		data[pos++] = pixel[3];
	}
}

#define BOTTOMUP(header) ((header).attrImage & 8)

void nextPixel(TGAHeader * header, int *pos)
{
	int components = header->bitsPerPixel / 8;
	*pos += components;
	if (BOTTOMUP(*header) && *pos % (header->width * components) == 0)
	{
		*pos -= 2 * header->width * components;
	}
}

static GLubyte readByte(FILE* file)
{
  return fgetc(file);
}

static GLushort readShort(FILE* file)
{
  int lower = readByte(file);
  int upper = readByte(file);
  return (upper << 8) | lower;
}

static bool readHeader(FILE* file, TGAHeader* header)
{
  header->lenID            = readByte(file);
  header->typePalette      = readByte(file);
  header->typeImage        = readByte(file);
  header->startPalette     = readShort(file);
  header->lenPalette       = readShort(file);
  header->sizePaletteEntry = readByte(file);
  header->startX           = readShort(file);
  header->startY           = readShort(file);
  header->width            = readShort(file);
  header->height           = readShort(file);
  header->bitsPerPixel     = readByte(file);
  header->attrImage        = readByte(file);

  return true;
}

Image::Image()
{
  // empty
}

Image::~Image()
{
  // empty
}

const char* Image::loadTGA(FILE* file)
{
	TGAHeader header;
	int compressed;
	int size;
	int pixels;

	if (!readHeader(file, &header))
	{
		return "header";
	}

	if (header.lenID != 0)
	{
		return "ID";
	}

	if (header.typePalette != 0)
	{
		return "Palette";
	}

	switch (header.typeImage)
	{
	case 10:
		compressed = 1;
		break;
	case 2:
		compressed = 0;
	default:
		return "Imagetype";
	}

	if (header.startX != 0 || header.startY != 0)
	{
		return "Offset";
	}

	width = header.width;
	height = header.height;

	switch (header.bitsPerPixel)
	{
	case 24:
		components = 3;
		format = GL_RGB;
		break;
	case 32:
		components = 4;
		format = GL_RGBA;
		break;
	default:
		return "Components";
	}

	pixels = width * height;
	size = pixels * components;

	data = new GLubyte[size];

	if (!data)
	{
		return "malloc";
	}

	if (compressed)
	{
		int pos = 0;
		int pixel = 0;

		if (BOTTOMUP(header))
		{
			pos = (height - 1) * width * components;
		}

		while (pixel < pixels)
		{
			int control = fgetc(file);
			if (control >> 7)
			{
				int repeat = (control & 0x7f) + 1;
				GLubyte value[4];
				int i;

				fread(value, 1, components, file);

				for (i = 0; i < repeat; i++)
				{
					copyPixel(data, pos, value, components);
					nextPixel(&header, &pos);
					pixel++;
				}
			}
			else
			{
				int plainbytes = ((control & 0x7f) + 1);
				GLubyte value[4];
				int i;

				for (i = 0; i < plainbytes; i++)
				{
					fread(value, 1, components, file);
					copyPixel(data, pos, value, components);
					nextPixel(&header, &pos);
					pixel++;
				}
			}
		}
	}
	else
	{
		return "data";
	}

	return NULL;
}
