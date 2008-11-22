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

#ifndef types_hpp
#define types_hpp

#include "kdtree/KdTree.hpp"

#include "FreeBlockList.hpp"

#include "math/Vector3.hpp"

#include GL_H

#define MAX_LEVEL_HEIGHT 10
#define HEIGHT_STEPS 10

#define MAX_SCORE_COLS 10
#define MAX_NAME_LENGTH 20

#define MIN_ALLOWED_CHAR 32
#define MAX_ALLOWED_CHAR 126

struct SubAtlas
{
	Orientation orientation;
	int idxSubLightMap;
};

struct FieldCoord
{
	int x;
	int y;
};

struct ScoreCol
{
	char name[MAX_NAME_LENGTH + 1];
	int tenthSecond;
};

struct Level
{
	FreeBlockList blocks;
	KdTree* kdLevelTree;
	FieldCoord start;
	FieldCoord finish;
	FieldCoord size;
	unsigned int borderTexture;
	GLuint colorMap;
	GLuint lightMap;

	const char* filename;
	char author[MAX_NAME_LENGTH + 1];
	unsigned int crc32;

	int cntScoreCols;
	ScoreCol scores[MAX_SCORE_COLS];
	int lastPlayerIndex;

	Vector3 origin;

	bool saved;
	bool waiting;
};

extern Level sgLevel;

#endif
