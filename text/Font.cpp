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

#include "Font.hpp"

#include "image/Image.hpp"

#include <GL/gl.h>

#define HEIGHT 128

Font::Font(const char* imagename)
{
	Image image;
	const char* error = image.loadTGA(imagename);

	if (error)
	{
		printf("error while loading %s: %s\n", imagename, error);
		return;
	}

	int index = 0;
	int sx = 0;
	int sy = 1;

	for (int ch = FONT_MIN_CHAR; ch <= FONT_MAX_CHAR; ch++)
	{
		int width = *image.pixel(index * 2, 0);
		int height = HEIGHT;
		float offsetY = *image.pixel(index * 2 + 1, 0) / (256.0f * 16.0f);

		float fWidth = (float) width / HEIGHT * 18.0f / 14.0f;
		float fHeight = 18.0f / 14.0f;

		if (sx + width > image.width())
		{
			sx = 0;
			sy += HEIGHT;
		}

		float fx = (float) sx / image.width();
		float fy = (float) sy / image.height();
		float fw = (float) width / image.width();
		float fh = (float) height / image.height();

		CharInfo& info = mInfo[ch - FONT_MIN_CHAR];

		info.coords[0] = Vector2(0.0f, 1.0f - offsetY);
		info.coords[1] = info.coords[0] - Vector2(0.0f, fHeight);
		info.coords[2] = info.coords[1] + Vector2(fWidth, 0.0f);
		info.coords[3] = info.coords[0] + Vector2(fWidth, 0.0f);

		info.texcoords[0] = Vector2(fx, fy);
		info.texcoords[1] = info.texcoords[0] + Vector2(0.0f, fh);
		info.texcoords[2] = info.texcoords[1] + Vector2(fw, 0.0f);
		info.texcoords[3] = info.texcoords[0] + Vector2(fw, 0.0f);

		index++;
		sx += width;
	}

	mTexID = image.toTexture(false);
}

Font::~Font()
{
	// empty
}

void Font::drawText(const char* str) const
{
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glEnable(GL_BLEND);
	{
		glBindTexture(GL_TEXTURE_2D, mTexID);
		glEnable(GL_TEXTURE_2D);
		{
			glPushMatrix();
			{
				for (const char* s = str; *s; s++)
				{
					drawChar(*s);
				}
			}
			glPopMatrix();
		}
		glDisable(GL_TEXTURE_2D);
	}
	glDisable(GL_BLEND);
}

void Font::drawChar(const char ch) const
{
	if (ch >= FONT_MIN_CHAR && ch <= FONT_MAX_CHAR)
	{
		const CharInfo& info = mInfo[ch - FONT_MIN_CHAR];

		glBegin(GL_QUADS);
		{
			for (int i = 0; i < 4; i++)
			{
				glTexCoord2fv(&info.texcoords[i].x);
				glVertex2fv(&info.coords[i].x);
			}
		}
		glEnd();

		glTranslatef(info.coords[3].x, 0.0f, 0.0f);
	}
}
