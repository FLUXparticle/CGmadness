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

#ifndef String_hpp
#define String_hpp

#include "SmartPointer.hpp"

class String
{
public:
	static String toString(int value);
	
public:
  String();
  String(const char* str);
  String(const String& str);
  virtual ~String();

  int length() const;

  operator const char*() const;
  bool operator==(const char* other) const;
  
  String operator+(const char* other) const;
  String operator+(const String& other) const;

private:
	SmartPointer<char> mStr;

};

inline String::operator const char*() const
{
	return mStr;
}

#endif
