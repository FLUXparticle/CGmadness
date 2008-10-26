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

#ifndef Font_hpp
#define Font_hpp

#include "math/Vector2.hpp"

#define FONT_MIN_CHAR 33
#define FONT_MAX_CHAR 126

class Font
{
public:
  Font(const char* imagename);
  virtual ~Font();

  float widthText(const char* str) const;
  void drawText(const char* str) const;

private:
	struct CharInfo
	{
		Vector2 coords[4];
		Vector2 texcoords[4];
	};

	CharInfo mInfo[FONT_MAX_CHAR - FONT_MIN_CHAR + 1];
	unsigned int mTexID;

	float widthChar(const char ch) const;
	void drawChar(const char ch) const;
	const CharInfo& getInfo(const char ch) const;

};

#endif
