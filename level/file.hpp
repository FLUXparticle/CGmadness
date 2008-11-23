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

#ifndef file_hpp
#define file_hpp

#include "types.hpp"

#include <cstdio>

int readByte(FILE* file);
int readInt(FILE* file);
void readString(FILE* file, char* str);
void readRLE(FILE* file, int* data);
void readFieldCoord(FILE* file, FieldCoord& coord);
void readFieldBlockV3(FILE* file, Block& block);
void readFieldBlock(FILE* file, Block& block);

void writeByte(FILE* file, int value);
void writeInt(FILE* file, int value);
void writeString(FILE* file, char* value);
void writeFieldCoord(FILE* file, const FieldCoord& coord);
void writeFieldBlock(FILE* file, const Block& block);
void writeRLE(FILE* file, const int* data);

#endif
