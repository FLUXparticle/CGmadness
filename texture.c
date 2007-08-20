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

#include "texture.h"

#include "debug.h"

#include <GL/glew.h>
#include <GL/glu.h>

#include <stdio.h>
#include <stdlib.h>

#define LINEAR_FILTER 1

static int gUseTextures = 0;

void initTextureEnvironment(void)
{
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
	gUseTextures = 1;
}

typedef struct
{
	GLubyte components;
	GLushort width;
	GLushort height;
	GLenum format;
	GLubyte* data;
} Image;

typedef struct
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
} __attribute__((__packed__)) TGAHeader;

void copyPixel(GLubyte* data, int pos, GLubyte* pixel, int components)
{
	data[pos++] = pixel[2];
	data[pos++] = pixel[1];
	data[pos++] = pixel[0];
	if (components > 3) {
		data[pos++] = pixel[3];
	}
}

#define BOTTOMUP(header) ((header).attrImage & 8)

void nextPixel(TGAHeader* header, int* pos)
{
	int components = header->bitsPerPixel / 8;
	*pos += components;
	if (BOTTOMUP(*header) && *pos % (header->width * components) == 0)
	{
		*pos -= 2 * header->width * components;
	}
}

int loadTGA(FILE* file, Image* image, char** error) {
	TGAHeader header;
	int compressed;
	int size;
	int pixels;

	if (fread(&header, sizeof(header), 1, file) != 1) {
		*error = "header";
		return 0;
	}

	if (header.lenID != 0) {
		*error = "ID";
		return 0;
	}

	if (header.typePalette != 0) {
		*error = "Palette";
		return 0;
	}

	switch (header.typeImage) {
	case 10:
		compressed = 1;
		break;
	case 2:
		compressed = 0;
	default:
		*error = "Imagetype";
		return 0;
	}

	if (header.startX != 0 || header.startY != 0) {
		*error = "Offset";
		return 0;
	}

	image->width = header.width;
	image->height = header.height;

	switch (header.bitsPerPixel) {
	case 24:
		image->components = 3;
		image->format = GL_RGB;
		break;
	case 32:
		image->components = 4;
		image->format = GL_RGBA;
		break;
	default:
		*error = "Components";
		return 0;
	}

	pixels = image->width * image->height;
	size = pixels * image->components;

	MALLOC(image->data, size);

	if (!image->data) {
		*error = "malloc";
		return 0;
	}

	if (compressed) {
		int pos = 0;
		int pixel = 0;

		if (BOTTOMUP(header)) {
			pos = (image->height - 1) * image->width * image->components;
		}

		while (pixel < pixels) {
			int control = fgetc(file);
			if (control >> 7) {
				int repeat = (control & 0x7f) + 1;
				GLubyte value[4];
				int i;

				fread(value, 1, image->components, file);

				for (i = 0; i < repeat; i++) {
					copyPixel(image->data, pos, value, image->components);
					nextPixel(&header, &pos);
					pixel++;
				}
			} else {
				int plainbytes = ((control & 0x7f) + 1);
				GLubyte value[4];
				int i;

				for (i = 0; i < plainbytes; i++) {
					fread(value, 1, image->components, file);
					copyPixel(image->data, pos, value, image->components);
					nextPixel(&header, &pos);
					pixel++;
				}
			}
		}
	} else {
		*error = "data";
		return 0;
	}

	return 1;
}

unsigned int genTexture(void)
{
	GLuint texID;

	glGenTextures(1, &texID);
	glBindTexture(GL_TEXTURE_2D, texID);

	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
#if LINEAR_FILTER
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
#else
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
#endif

	return texID;
}

int loadTexture(const char* filename, int mipmapping) {
	GLuint id;
	Image image;
	FILE* file = fopen(filename, "rb");
	int success = 0;
	char* error = NULL;

	if (file) {
		success = loadTGA(file, &image, &error);
		fclose(file);
	} else {
		error = "open";
	}

	if (!success) {
		printf("%s: %s\n", filename, error);
		return -1;
	}

	if (!gUseTextures) {
		initTextureEnvironment();
	}

	glGenTextures(1, &id);
	glBindTexture(GL_TEXTURE_2D, id);

	if (mipmapping) {
		gluBuild2DMipmaps(GL_TEXTURE_2D, image.components, image.width, image.height, image.format, GL_UNSIGNED_BYTE, image.data);
	} else {
		glTexImage2D(GL_TEXTURE_2D, 0, image.components, image.width, image.height, 0, image.format, GL_UNSIGNED_BYTE, image.data);
	}

	FREE(image.data);

	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	if (mipmapping) {
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	} else {
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	}

	return id;
}
