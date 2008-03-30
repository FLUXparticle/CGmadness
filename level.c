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

#include "level.h"

#include "common.h"
#include "atlas.h"
#include "crc32.h"
#include "tools.h"

#include "functions.h"
#include "debug.h"

#include <GL/glut.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#define THIS_CGM_VERSION 3
#define THIS_HIGHSCORE_VERSION 1

#define EXT_HIGHSCORE ".highscore"

#define MAX_LEVEL_SIZE 100

const int sgEdgeX[4] = { 0, 1, 1, 0 };
const int sgEdgeY[4] = { 0, 0, 1, 1 };

Level sgLevel;

static void updateSquareAttributes(Square * square)
{
	int i;

	for (i = 0; i < 4; i++)
	{
		square->vertices[i] = add(square->vertices[i], sgLevel.origin);
	}

	square->mid = midpoint(square->vertices);

	square->area =
		0.5f *
		len(cross
				(sub(square->vertices[1], square->vertices[0]),
				 sub(square->vertices[3],
						 square->vertices[0]))) + 0.5f * len(cross(sub(square->vertices[1],
																													 square->vertices[2]),
																											 sub(square->vertices[3],
																													 square->
																													 vertices[2])));
}

static int getFieldEdgeHeight(int x, int y, int edge)
{
	if (x >= 0 && y >= 0 && x < sgLevel.size.x && y < sgLevel.size.y)
	{
		Plate *p = &sgLevel.field[x][y];

		return 5 + p->z + (sgEdgeX[edge] * 2 - 1) * p->dzx + (sgEdgeY[edge] * 2 -
																													1) * p->dzy;
	}

	/* value out of range */
	return 0;
}

void updatePlate(int x, int y)
{
	Plate *p = &sgLevel.field[x][y];

	if (p->dirty)
	{
		Square *square = &p->roof;
		int i;
		int side;
		int dzx = p->dzx;
		int dzy = p->dzy;

		Vector3 ex;
		Vector3 ey;

		ex.x = 0.5f;
		ex.y = 0.0f;
		ex.z = (float) dzx / HEIGHT_STEPS;

		ey.x = 0.0f;
		ey.y = 0.5f;
		ey.z = (float) dzy / HEIGHT_STEPS;

		square->normal = norm(cross(ex, ey));

		for (i = 0; i < 4; i++)
		{
			square->vertices[i].x = x + sgEdgeX[i];
			square->vertices[i].y = y + sgEdgeY[i];
			square->vertices[i].z =
				(float) getFieldEdgeHeight(x, y, i) / HEIGHT_STEPS;
		}

		updateSquareAttributes(square);

		for (side = 0; side < 4; side++)
		{
			int next = (side + 1) % 4;
			int prev = (side + 3) % 4;

			int sideOpposite = (side + 3) % 4;
			int nextOpposite = (side + 2) % 4;

			int dx = sgEdgeX[side] - sgEdgeX[prev];
			int dy = sgEdgeY[side] - sgEdgeY[prev];

			int height1 = getFieldEdgeHeight(x, y, side);
			int height2 = getFieldEdgeHeight(x, y, next);

			int height3 = getFieldEdgeHeight(x + dx, y + dy, sideOpposite);
			int height4 = getFieldEdgeHeight(x + dx, y + dy, nextOpposite);

			SideFace *face = &p->sideFaces[side];

			face->cntSquares = 0;

			if (height1 > height3 || height2 > height4)
			{
				int x1 = x + sgEdgeX[side];
				int y1 = y + sgEdgeY[side];
				int x2 = x + sgEdgeX[next];
				int y2 = y + sgEdgeY[next];

				int startHeight = min(height3, height4);

				int minHeight;
				int maxHeight;
				int x3;
				int y3;

				int bottom;

				if (height1 > height2)
				{
					minHeight = height2;
					maxHeight = height1;
					x3 = x1;
					y3 = y1;
				}
				else
				{
					minHeight = height1;
					maxHeight = height2;
					x3 = x2;
					y3 = y2;
				}

				face->bottom = (float) startHeight / HEIGHT_STEPS;
				face->top = (float) maxHeight / HEIGHT_STEPS;

				for (bottom = startHeight; bottom < minHeight;)
				{
					int top =
						min(minHeight, bottom - (bottom % HEIGHT_STEPS) + HEIGHT_STEPS);

					square = &face->squares[face->cntSquares++];

					square->normal.x = dx;
					square->normal.y = dy;
					square->normal.z = 0.0f;

					square->vertices[0].x = x1;
					square->vertices[0].y = y1;
					square->vertices[0].z = (float) top / HEIGHT_STEPS;

					square->vertices[1].x = x1;
					square->vertices[1].y = y1;
					square->vertices[1].z = (float) bottom / HEIGHT_STEPS;

					square->vertices[2].x = x2;
					square->vertices[2].y = y2;
					square->vertices[2].z = (float) bottom / HEIGHT_STEPS;

					square->vertices[3].x = x2;
					square->vertices[3].y = y2;
					square->vertices[3].z = (float) top / HEIGHT_STEPS;

					updateSquareAttributes(square);

					bottom = top;
				}

				for (; bottom < maxHeight;)
				{
					int top =
						min(maxHeight, bottom - (bottom % HEIGHT_STEPS) + HEIGHT_STEPS);

					float t1 = (float) (bottom - minHeight) / (maxHeight - minHeight);
					float t2 = (float) (top - minHeight) / (maxHeight - minHeight);

					square = &face->squares[face->cntSquares++];

					square->normal.x = dx;
					square->normal.y = dy;
					square->normal.z = 0.0f;

					square->vertices[0].x = x1 * (1.0f - t2) + x3 * t2;
					square->vertices[0].y = y1 * (1.0f - t2) + y3 * t2;
					square->vertices[0].z = (float) top / HEIGHT_STEPS;

					square->vertices[1].x = x1 * (1.0f - t1) + x3 * t1;
					square->vertices[1].y = y1 * (1.0f - t1) + y3 * t1;
					square->vertices[1].z = (float) bottom / HEIGHT_STEPS;

					square->vertices[2].x = x2 * (1.0f - t1) + x3 * t1;
					square->vertices[2].y = y2 * (1.0f - t1) + y3 * t1;
					square->vertices[2].z = (float) bottom / HEIGHT_STEPS;

					square->vertices[3].x = x2 * (1.0f - t2) + x3 * t2;
					square->vertices[3].y = y2 * (1.0f - t2) + y3 * t2;
					square->vertices[3].z = (float) top / HEIGHT_STEPS;

					updateSquareAttributes(square);

					bottom = top;
				}
			}

		}

		p->dirty = 0;
	}
}

float getMaxZValue(const Square * square)
{
	int i;
	float res = square->vertices[0].z;
	for (i = 1; i < 4; i++)
	{
		if (square->vertices[i].z > res)
		{
			res = square->vertices[i].z;
		}
	}
	return res;
}

void initLevel(void)
{
	int x;

	MALLOC(sgLevel.field, sgLevel.size.x * sizeof(Plate *));
	MALLOC(sgLevel.field[0], sgLevel.size.x * sgLevel.size.y * sizeof(Plate));
	for (x = 1; x < sgLevel.size.x; x++)
	{
		sgLevel.field[x] = &sgLevel.field[x - 1][sgLevel.size.y];
	}

	sgLevel.origin.x = -sgLevel.size.x / 2.0f;
	sgLevel.origin.y = 10.0f;
	sgLevel.origin.z = 0.0f;
}

void destroyLevel(void)
{
	FREE(sgLevel.field[0]);
	FREE(sgLevel.field);

	sgLevel.size.x = -1;
	sgLevel.size.y = -1;

	destroyCommon();
}

void getRoofSquare(int x, int y, Square * square)
{
	Plate *p = &sgLevel.field[x][y];
	updatePlate(x, y);
	*square = p->roof;
}

void getSideFace(int x, int y, int side, SideFace * face)
{
	Plate *p = &sgLevel.field[x][y];
	updatePlate(x, y);

	*face = p->sideFaces[side];
}

void newLevel(void)
{
	int x, y;

	fprintf(stderr, "creating new level: (%d, %d)\n", sgLevel.size.x,
					sgLevel.size.y);

	sgLevel.start.x = 0;
	sgLevel.start.y = 0;

	sgLevel.finish.x = sgLevel.size.x - 1;
	sgLevel.finish.y = sgLevel.size.y - 1;

	initLevel();

	for (x = 0; x < sgLevel.size.x; x++)
	{
		for (y = 0; y < sgLevel.size.y; y++)
		{
			Plate *p = &sgLevel.field[x][y];
			p->z = 0;
			p->dzx = 0;
			p->dzy = 0;
			p->dirty = 1;
		}
	}

	initCommon();

	updateLightMap(0);
}

static int readByte(FILE * file)
{
	int value;

	fscanf(file, "%i", &value);
	nextByte(value);

	return value;
}

int readInt(FILE * file)
{
	int value;
	int tmp;
	int i;

	fscanf(file, "%i", &value);

	tmp = value;

	for (i = 0; i < 4; i++)
	{
		nextByte(tmp);
		tmp >>= 8;
	}

	return value;
}

void readFieldCoord(FILE * file, FieldCoord * coord)
{
	coord->x = readByte(file);
	coord->y = readByte(file);
}

void readFieldPlate(FILE * file, Plate * plate)
{
	plate->z = readByte(file);
	plate->dzx = readByte(file);
	plate->dzy = readByte(file);
}

void readRLEByte(FILE * file, int *repeat, int *value)
{
	int i;

	if (fscanf(file, "%ix%i", repeat, value) < 2)
	{
		*value = *repeat;
		*repeat = 1;
	}

	for (i = 0; i < *repeat; i++)
	{
		nextByte(*value);
	}
}

void readRLE(FILE * file, int data[SIZEOF_LIGHT_MAP])
{
	int s = 0;
	int i;

	while (s < SIZEOF_LIGHT_MAP)
	{
		int repeat;
		int value;

		readRLEByte(file, &repeat, &value);

		for (i = 0; i < repeat; i++)
		{
			data[s + i] = value;
		}

		s += repeat;
	}
}

void readString(FILE * file, char *str)
{
	int i;

	char *s = str;

	fgetc(file);

	for (i = 0; i < MAX_NAME_LENGTH; i++)
	{
		int b = fgetc(file);

		if (b < MIN_ALLOWED_CHAR)
		{
			break;
		}

		*s = b;
		nextByte(b);

		s++;
	}

	*s = '\0';
}


void toLightMap(int index, int flip, int dataInt[SIZEOF_LIGHT_MAP])
{
	GLfloat dataFloat[SIZEOF_LIGHT_MAP];
	int i;

	if (flip)
	{
		for (i = 0; i < SIZEOF_LIGHT_MAP; i++)
		{
			dataFloat[i + LIGHT_MAP_SIZE - 1 - 2 * (i % LIGHT_MAP_SIZE)] =
				(float) dataInt[i] / 255;
		}
	}
	else
	{
		for (i = 0; i < SIZEOF_LIGHT_MAP; i++)
		{
			dataFloat[i] = (float) dataInt[i] / 255;
		}
	}

	setSubLightMap(index, dataFloat);
}

int loadHighscoreFromFile(void)
{
	int i;
	unsigned int version;
	unsigned int crc32;

	char *filename = addStrings(sgLevel.filename, EXT_HIGHSCORE);
	FILE *file = fopen(filename, "rt");
	int result = 1;

	FREE(filename);

	if (!file)
		return 0;

	/* version number */
	fscanf(file, "v%u", &version);

	if (version > THIS_HIGHSCORE_VERSION)
	{
		fprintf(stderr, "incompatible version number: %u\n", version);
		fclose(file);
		return 0;
	}

	fscanf(file, "%x\n", &crc32);

	if (crc32 != sgLevel.crc32)
	{
		fprintf(stderr, "1st checksum mismatch: %s\n", filename);
		fclose(file);
		return 0;
	}

	setCRC32(sgLevel.crc32);

	sgLevel.cntScoreCols = readByte(file);

	for (i = 0; i < sgLevel.cntScoreCols; i++)
	{
		sgLevel.scores[i].tenthSecond = readInt(file);
		readString(file, sgLevel.scores[i].name);
	}

	fscanf(file, "%x\n", &crc32);

	if (crc32 != getCRC32())
	{
		fprintf(stderr, "2st checksum mismatch: %s\n", filename);
		fclose(file);
		return 0;
	}

	fclose(file);

	return result;
}

int loadLevelFromFile(const char *filename, int justLoad)
{
	FILE *file = fopen(filename, "rt");
	int result = 1;

	int x, y;
	FieldCoord fileCoords;
	unsigned int version;
	unsigned int crc32;
	int resize = 1;

	sgLevel.filename = filename;

	if (!file)
	{
		if (between(sgLevel.size.x, 1, MAX_LEVEL_SIZE)
				&& between(sgLevel.size.y, 1, MAX_LEVEL_SIZE))
		{
			newLevel();
			return 1;
		}
		else
		{
			fprintf(stderr, "can not open file: %s\n", filename);
			return 0;
		}
	}

	/* version number */
	fscanf(file, "v%u", &version);

	if (version > THIS_CGM_VERSION)
	{
		fprintf(stderr, "incompatible version number: %u\n", version);
		fclose(file);
		return 0;
	}

	resetCRC32();

	/* read attributes */
	readFieldCoord(file, &sgLevel.start);
	readFieldCoord(file, &sgLevel.finish);
	readFieldCoord(file, &fileCoords);

	/* read size from file, if not given through program parameters */
	if (justLoad || sgLevel.size.x < 0 || sgLevel.size.y < 0)
	{
		sgLevel.size = fileCoords;
		resize = 0;
	}

	initLevel();

	/* reading data */
	for (x = 0; x < sgLevel.size.x; x++)
	{
		for (y = 0; y < sgLevel.size.y; y++)
		{
			Plate *p = &sgLevel.field[x][y];
			if (x < fileCoords.x && y < fileCoords.y)
			{
				readFieldPlate(file, p);
			}
			else
			{													/* growing */
				p->z = 0;
				p->dzx = 0;
				p->dzy = 0;
			}

			p->dirty = 1;
		}

		/* shrinking */
		if (fileCoords.y > sgLevel.size.y)
		{
			Plate dummyPlate;

			for (y = sgLevel.size.y; y < fileCoords.y; y++)
			{
				readFieldPlate(file, &dummyPlate);
			}
		}
	}

	sgLevel.crc32 = getCRC32();

	initCommon();

	if (version >= 2 && !resize)
	{
		fscanf(file, "%x\n", &crc32);

		if (crc32 != getCRC32())
		{
			fprintf(stderr, "1st checksum mismatch: %s\n", filename);
			fclose(file);
			return 0;
		}

		{
			int cntSubLightMaps = getCntAllocatedSubLightMaps();

			int index = 0;

			if (version >= 3)
			{
				while (index < cntSubLightMaps)
				{
					int dataInt[SIZEOF_LIGHT_MAP];

					readRLE(file, dataInt);

					toLightMap(index, 0, dataInt);

					index++;
				}
			}
			else
			{
				const int start = sgLevel.size.x * sgLevel.size.y;
				const int cntTmpSubMaps =
					start + 4 * sgLevel.size.x * sgLevel.size.y * (MAX_LEVEL_HEIGHT + 1);

				int index = 0;

				typedef int LightMap[SIZEOF_LIGHT_MAP];
				LightMap *tmp;
				int i;

				int starts[4];
				int side;

				starts[0] =
					start + 2 * sgLevel.size.x * sgLevel.size.y * (MAX_LEVEL_HEIGHT + 1);
				starts[1] = start;
				starts[2] = starts[0] + sgLevel.size.x * (MAX_LEVEL_HEIGHT + 1);
				starts[3] = starts[1] + sgLevel.size.y * (MAX_LEVEL_HEIGHT + 1);

				MALLOC(tmp, cntTmpSubMaps * sizeof(*tmp));

				for (i = 0; i < cntTmpSubMaps; i++)
				{
					readRLE(file, tmp[i]);
				}

				for (x = 0; x < sgLevel.size.x; x++)
				{
					for (y = 0; y < sgLevel.size.y; y++)
					{
						toLightMap(index++, 0, tmp[y * sgLevel.size.x + x]);

						for (side = 0; side < 4; side++)
						{
							int bottom = 0;
							int top = 0;

							SideFace face;

							getSideFace(x, y, side, &face);

							if (face.cntSquares > 0)
							{
								bottom = (int) floor(face.bottom);
								top = (int) ceil(face.top);
							}

							if (side % 2 == 0)
							{
								int j;
								for (j = bottom; j < top; j++)
								{
									toLightMap(index++, side / 2,
														 tmp[starts[side] +
																 y * 2 * sgLevel.size.x * (MAX_LEVEL_HEIGHT +
																													 1) +
																 j * sgLevel.size.x + x]);
								}
							}
							else
							{
								int j;
								for (j = bottom; j < top; j++)
								{
									toLightMap(index++, side / 2,
														 tmp[starts[side] +
																 x * 2 * sgLevel.size.y * (MAX_LEVEL_HEIGHT +
																													 1) +
																 j * sgLevel.size.y + y]);
								}
							}
						}
					}
				}

				FREE(tmp);
			}

			fscanf(file, "%x\n", &crc32);

			if (crc32 != getCRC32())
			{
				fprintf(stderr, "2st checksum mismatch: %s\n", filename);
				fclose(file);
				return 0;
			}
		}
	}
	else
	{
		updateLightMap(0);
	}

	fclose(file);

	if (!loadHighscoreFromFile())
	{
		sgLevel.cntScoreCols = 0;
	}

	sgLevel.saved = 1;

	return result;
}

void writeByte(FILE * file, int value)
{
	fprintf(file, "%i", value);
	nextByte(value);
}

void writeInt(FILE * file, int value)
{
	int i;

	fprintf(file, "%i", value);

	for (i = 0; i < 4; i++)
	{
		nextByte(value);
		value >>= 8;
	}
}

void writeString(FILE * file, char *value)
{
	const char *s;

	for (s = value; *s; s++)
	{
		fputc(*s, file);
		nextByte(*s);
	}
}

void writeFieldCoord(FILE * file, const FieldCoord coord)
{
	writeByte(file, coord.x);
	fputc(' ', file);
	writeByte(file, coord.y);
	fputc('\n', file);
}

void writeFieldPlate(FILE * file, const Plate * plate)
{
	writeByte(file, plate->z);
	fputc(' ', file);
	writeByte(file, plate->dzx);
	fputc(' ', file);
	writeByte(file, plate->dzy);
	fputc('\n', file);
}

void writeRLEInt(FILE * file, int repeat, int value)
{
	int i;

	if (repeat > 1)
	{
		fprintf(file, "%ix%i", repeat, value);
	}
	else
	{
		fprintf(file, "%i", value);
	}

	for (i = 0; i < repeat; i++)
	{
		nextByte(value);
	}
}

void writeRLE(FILE * file, const int data[SIZEOF_LIGHT_MAP])
{
	int s = 0;
	int l = 1;

	while (s < SIZEOF_LIGHT_MAP)
	{
		if (s + l < SIZEOF_LIGHT_MAP && data[s] == data[s + l])
		{
			l++;
		}
		else
		{
			writeRLEInt(file, l, data[s]);

			fputc(' ', file);
			s += l;
			l = 1;
		}
	}
	fputc('\n', file);
}

int saveHighscoreToFile(void)
{
	int i;

	char *filename = addStrings(sgLevel.filename, EXT_HIGHSCORE);
	FILE *file = fopen(filename, "wt");

	if (!file)
		return 0;

	/* version number */
	fprintf(file, "v%u\n", THIS_HIGHSCORE_VERSION);

	fprintf(file, "%08X\n", sgLevel.crc32);

	setCRC32(sgLevel.crc32);

	writeByte(file, sgLevel.cntScoreCols);

	fputc('\n', file);

	for (i = 0; i < sgLevel.cntScoreCols; i++)
	{
		writeInt(file, sgLevel.scores[i].tenthSecond);
		fputc(' ', file);
		writeString(file, sgLevel.scores[i].name);
		fputc('\n', file);
	}

	fprintf(file, "%08X\n", getCRC32());

	FREE(filename);

	if (fclose(file) != 0)
	{
		return 0;
	}

	return 1;
}

int saveLevelToFile(void)
{
	FILE *file = fopen(sgLevel.filename, "wt");

	int x, y;

	if (!file)
		return 0;

	/* version number */
	fprintf(file, "v%u\n", THIS_CGM_VERSION);

	resetCRC32();

	/* write atributes */
	writeFieldCoord(file, sgLevel.start);
	writeFieldCoord(file, sgLevel.finish);
	writeFieldCoord(file, sgLevel.size);

	/* write data */
	for (x = 0; x < sgLevel.size.x; x++)
	{
		for (y = 0; y < sgLevel.size.y; y++)
		{
			Plate *p = &sgLevel.field[x][y];
			writeFieldPlate(file, p);
		}
	}

	fprintf(file, "%08X\n", getCRC32());

	{
		int cntSubLightMaps = getCntAllocatedSubLightMaps();

		int i;
		int j;

		for (i = 0; i < cntSubLightMaps; i++)
		{
			GLfloat dataFloat[SIZEOF_LIGHT_MAP];
			int dataInt[SIZEOF_LIGHT_MAP];

			getSubLightMap(i, dataFloat);

			for (j = 0; j < SIZEOF_LIGHT_MAP; j++)
			{
				dataInt[j] = (int) (clamp(dataFloat[j], 0.0f, 1.0f) * 255);
			}

			writeRLE(file, dataInt);
		}

		fprintf(file, "%08X\n", getCRC32());
	}

	if (fclose(file) != 0)
	{
		return 0;
	}

	return 1;
}
