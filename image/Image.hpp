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

#ifndef Image_hpp
#define Image_hpp

#include GL_H

#include <fstream>

class Image
{
public:
	Image();
  virtual ~Image();

  const char* loadTGA(const char* filename);
  GLuint toTexture(bool mipmapping);

  const GLubyte* pixel(int x, int y) const;

  int width() const;
  int height() const;

private:
	GLubyte mComponents;
	GLushort mWidth;
	GLushort mHeight;
	GLenum mFormat;
	GLubyte* mData;

  const char* loadTGA(std::ifstream& file);

};

inline int Image::width() const
{
	return mWidth;
}

inline int Image::height() const
{
	return mHeight;
}

#endif
