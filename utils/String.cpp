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

#include "String.hpp"

#include <stdio.h>
#include <string.h>

char* copy(const char* str)
{
	int size = strlen(str) + 1;
	char* result = new char[size];
	strcpy(result, str);
	return result;
}

String String::toString(int value)
{
	char tmp[128];
	
	sprintf(tmp, "%d", value);
	
	return String(tmp);
}

String::String(const char* str) :
	mStr(copy(str))
{
	// empty
}

String::String(const String& other) :
	mStr(copy(other.mStr))
{
	// empty
}

String::String() :
	mStr(new char(0))
{
}

String::~String()
{
	// empty
}

int String::length() const
{
	return strlen(mStr);
}

bool String::operator==(const char* other) const
{
	return strcmp(mStr, other) == 0;
}

String String::operator+(const char* other) const
{
	String newString;
	
	newString.mStr = new char[length() + strlen(other) + 1];
	
	strcpy(newString.mStr, mStr);
	strcat(newString.mStr, other);
	
	return newString;
}

String String::operator+(const String& other) const
{
	return operator+(other.mStr);
}
