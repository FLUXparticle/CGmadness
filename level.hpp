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

#ifndef _level_hpp_
#define _level_hpp_

#include "vector.hpp"

#define MAX_LEVEL_HEIGHT 10
#define HEIGHT_STEPS 10

#define MAX_SCORE_COLS 10
#define MAX_NAME_LENGTH 20

#define MIN_ALLOWED_CHAR 32
#define MAX_ALLOWED_CHAR 127

#define NOISE_TEXTURE 0

typedef struct
{
	Vector3 normal;
	Vector2 colormap[4];
	Vector2 lightmap[4];
	Vector2 texcoord[4];
	Vector3 vertices[4];

	float area;
	Vector3 mid;
} Square;

typedef struct
{
	int cntSquares;
	Square squares[MAX_LEVEL_HEIGHT + 2];

	float bottom;
	float top;
} SideFace;

typedef struct
{
	int z;
	int dzx;
	int dzy;

	int dirty;
	Square roof;

	SideFace sideFaces[4];
} Plate;

typedef struct
{
	int x;
	int y;
} FieldCoord;

typedef struct
{
	char name[MAX_NAME_LENGTH + 1];
	int tenthSecond;
} ScoreCol;

typedef struct
{
	Plate **field;
	FieldCoord start;
	FieldCoord finish;
	FieldCoord size;
	unsigned int borderTexture;
	unsigned int colorMap;
	unsigned int lightMap;

	const char *filename;
	unsigned int crc32;

	int cntScoreCols;
	ScoreCol scores[MAX_SCORE_COLS];

	Vector3 origin;

	int saved;
	int waiting;
} Level;

extern const int sgEdgeX[4];
extern const int sgEdgeY[4];

extern Level sgLevel;

void initLevel(void);
void destroyLevel(void);

void newLevel(void);

int loadHighscoreFromFile(void);
int loadLevelFromFile(const char *filename, bool justLoad);

int saveHighscoreToFile(void);
int saveLevelToFile(void);

void updatePlate(int x, int y);
void getRoofSquare(int x, int y, Square * square);
void getSideFace(int x, int y, int side, SideFace * face);

float getMaxZValue(const Square * square);

#endif
