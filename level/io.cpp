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

#include "io.hpp"

#include "types.hpp"
#include "crc32.hpp"
#include "level.hpp"

#include "atlas.hpp"
#include "common.hpp"

#include "utils/String.hpp"

#include "functions.hpp"

#include <cstdio>
#include <cmath>

#define EXT_HIGHSCORE ".highscore"

#define THIS_CGM_VERSION 3
#define THIS_HIGHSCORE_VERSION 1

#define MAX_LEVEL_SIZE 100

static int readByte(FILE* file)
{
	int value = -1;

	if (fscanf(file, "%i", &value) == 1)
	{
		nextByte(value);
	}

	return value;
}

static int readInt(FILE* file)
{
	int value = -1;

	if (fscanf(file, "%i", &value) == 1)
	{
		int tmp = value;

		for (int i = 0; i < 4; i++)
		{
			nextByte(tmp);
			tmp >>= 8;
		}
	}

	return value;
}

void readFieldCoord(FILE* file, FieldCoord* coord)
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
	if (fscanf(file, "%ix%i", repeat, value) < 2)
	{
		*value = *repeat;
		*repeat = 1;
	}

	for (int i = 0; i < *repeat; i++)
	{
		nextByte(*value);
	}
}

void readRLE(FILE* file, int data[SIZEOF_LIGHT_MAP])
{
	int s = 0;

	while (s < SIZEOF_LIGHT_MAP)
	{
		int repeat;
		int value;

		readRLEByte(file, &repeat, &value);

		for (int i = 0; i < repeat; i++)
		{
			data[s + i] = value;
		}

		s += repeat;
	}
}

void readString(FILE * file, char *str)
{
	char *s = str;

	fgetc(file);

	for (int i = 0; i < MAX_NAME_LENGTH; i++)
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

	if (flip)
	{
		for (int i = 0; i < SIZEOF_LIGHT_MAP; i++)
		{
			dataFloat[i + LIGHT_MAP_SIZE - 1 - 2 * (i % LIGHT_MAP_SIZE)] =
				(float) dataInt[i] / 255;
		}
	}
	else
	{
		for (int i = 0; i < SIZEOF_LIGHT_MAP; i++)
		{
			dataFloat[i] = (float) dataInt[i] / 255;
		}
	}

	setSubLightMap(index, dataFloat);
}

bool loadHighscoreFromFile(void)
{
	unsigned int version;
	unsigned int crc32;

	String filename = String(sgLevel.filename) + EXT_HIGHSCORE;
	FILE *file = fopen(filename, "rt");

	if (!file)
	{
		return false;
	}

	/* version number */
	if (fscanf(file, "v%u", &version) != 1 || version > THIS_HIGHSCORE_VERSION)
	{
		fprintf(stderr, "incompatible version number: %u\n", version);
		fclose(file);
		return false;
	}

	if (fscanf(file, "%x\n", &crc32) != 1 || crc32 != sgLevel.crc32)
	{
		fprintf(stderr, "1st checksum mismatch: %s\n", (const char*) filename);
		fclose(file);
		return false;
	}

	setCRC32(sgLevel.crc32);

	sgLevel.cntScoreCols = readByte(file);

	for (int i = 0; i < sgLevel.cntScoreCols; i++)
	{
		sgLevel.scores[i].tenthSecond = readInt(file);
		readString(file, sgLevel.scores[i].name);
	}

	if (fscanf(file, "%x\n", &crc32) != 1 || crc32 != getCRC32())
	{
		fprintf(stderr, "2st checksum mismatch: %s\n", (const char*) filename);
		fclose(file);
		return false;
	}

	fclose(file);

	return true;
}

void importLightmapV2(FILE* file)
{
	typedef int LightMap[SIZEOF_LIGHT_MAP];

	const int start = sgLevel.size.x * sgLevel.size.y;

	int starts[4];

	starts[0] = start + 2 * sgLevel.size.x * sgLevel.size.y * (MAX_LEVEL_HEIGHT + 1);
	starts[1] = start;
	starts[2] = starts[0] + sgLevel.size.x * (MAX_LEVEL_HEIGHT + 1);
	starts[3] = starts[1] + sgLevel.size.y * (MAX_LEVEL_HEIGHT + 1);

	const int cntTmpSubMaps = start + 4 * sgLevel.size.x * sgLevel.size.y * (MAX_LEVEL_HEIGHT + 1);

	LightMap* tmp = new LightMap[cntTmpSubMaps];

	for (int i = 0; i < cntTmpSubMaps; i++)
	{
		readRLE(file, tmp[i]);
	}

	int index = 0;

	for (int x = 0; x < sgLevel.size.x; x++)
	{
		for (int y = 0; y < sgLevel.size.y; y++)
		{
			toLightMap(index++, 0, tmp[y * sgLevel.size.x + x]);

			for (int side = 0; side < 4; side++)
			{
				int bottom = 0;
				int top = 0;

				const SideFace& face = getSideFace(x, y, side);

				if (!face.squares.empty())
				{
					bottom = face.bottom;
					top = face.top;
				}

				if (side % 2 == 0)
				{
					for (int j = bottom; j < top; j++)
					{
						int indexTmp = starts[side] + y * 2 * sgLevel.size.x * (MAX_LEVEL_HEIGHT + 1) + j * sgLevel.size.x + x;
						toLightMap(index++, side / 2, tmp[indexTmp]);
					}
				}
				else
				{
					for (int j = bottom; j < top; j++)
					{
						int indexTmp = starts[side] + x * 2 * sgLevel.size.y * (MAX_LEVEL_HEIGHT + 1) + j * sgLevel.size.y + y;
						toLightMap(index++, side / 2, tmp[indexTmp]);
					}
				}
			}
		}
	}

	delete[] tmp;
}

void newLevel()
{
	fprintf(stderr, "creating new level: (%d, %d)\n", sgLevel.size.x,
	    sgLevel.size.y);

	sgLevel.start.x = 0;
	sgLevel.start.y = 0;

	sgLevel.finish.x = sgLevel.size.x - 1;
	sgLevel.finish.y = sgLevel.size.y - 1;

	initLevel();

	for (int x = 0; x < sgLevel.size.x; x++)
	{
		for (int y = 0; y < sgLevel.size.y; y++)
		{
			Plate* p = &sgLevel.field[x][y];
			p->z = 0;
			p->dzx = 0;
			p->dzy = 0;
			p->block.dirty = true;
		}
	}

	initCommon();

	updateLightMap(0);
}

bool loadLevelFromFile(const char* filename, bool justLoad)
{
	FILE* file = fopen(filename, "rt");

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
			return true;
		}
		else
		{
			fprintf(stderr, "can not open file: %s\n", filename);
			return false;
		}
	}

	/* version number */
	if (fscanf(file, "v%u", &version) != 1 || version > THIS_CGM_VERSION)
	{
		fprintf(stderr, "incompatible version number: %u\n", version);
		fclose(file);
		return false;
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
	for (int x = 0; x < sgLevel.size.x; x++)
	{
		for (int y = 0; y < sgLevel.size.y; y++)
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

			p->block.dirty = true;
		}

		/* shrinking */
		if (fileCoords.y > sgLevel.size.y)
		{
			Plate dummyPlate;

			for (int y = sgLevel.size.y; y < fileCoords.y; y++)
			{
				readFieldPlate(file, &dummyPlate);
			}
		}
	}

	sgLevel.crc32 = getCRC32();

	initCommon();

	if (version >= 2 && !resize)
	{
		if (fscanf(file, "%x\n", &crc32) != 1 || crc32 != getCRC32())
		{
			fprintf(stderr, "1st checksum mismatch: %s\n", filename);
			fclose(file);
			return false;
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
				importLightmapV2(file);
			}

			if (fscanf(file, "%x\n", &crc32) != 1 || crc32 != getCRC32())
			{
				fprintf(stderr, "2st checksum mismatch: %s\n", filename);
				fclose(file);
				return false;
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

	sgLevel.saved = true;

	return true;
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

bool saveHighscoreToFile(void)
{
	String filename = String(sgLevel.filename) + EXT_HIGHSCORE;
	FILE* file = fopen(filename, "wt");

	if (!file)
	{
		return false;
	}

	/* version number */
	fprintf(file, "v%u\n", THIS_HIGHSCORE_VERSION);

	fprintf(file, "%08X\n", sgLevel.crc32);

	setCRC32(sgLevel.crc32);

	writeByte(file, sgLevel.cntScoreCols);

	fputc('\n', file);

	for (int i = 0; i < sgLevel.cntScoreCols; i++)
	{
		writeInt(file, sgLevel.scores[i].tenthSecond);
		fputc(' ', file);
		writeString(file, sgLevel.scores[i].name);
		fputc('\n', file);
	}

	fprintf(file, "%08X\n", getCRC32());

	if (fclose(file) != 0)
	{
		return false;
	}

	return true;
}

bool saveLevelToFile(void)
{
	FILE *file = fopen(sgLevel.filename, "wt");

	if (!file)
	{
		return false;
	}

	/* version number */
	fprintf(file, "v%u\n", THIS_CGM_VERSION);

	resetCRC32();

	/* write atributes */
	writeFieldCoord(file, sgLevel.start);
	writeFieldCoord(file, sgLevel.finish);
	writeFieldCoord(file, sgLevel.size);

	/* write data */
	for (int x = 0; x < sgLevel.size.x; x++)
	{
		for (int y = 0; y < sgLevel.size.y; y++)
		{
			Plate *p = &sgLevel.field[x][y];
			writeFieldPlate(file, p);
		}
	}

	fprintf(file, "%08X\n", getCRC32());

	{
		int cntSubLightMaps = getCntAllocatedSubLightMaps();

		for (int i = 0; i < cntSubLightMaps; i++)
		{
			GLfloat dataFloat[SIZEOF_LIGHT_MAP];
			int dataInt[SIZEOF_LIGHT_MAP];

			getSubLightMap(i, dataFloat);

			for (int j = 0; j < SIZEOF_LIGHT_MAP; j++)
			{
				dataInt[j] = (int) (clamp(dataFloat[j], 0.0f, 1.0f) * 255);
			}

			writeRLE(file, dataInt);
		}

		fprintf(file, "%08X\n", getCRC32());
	}

	if (fclose(file) != 0)
	{
		return false;
	}

	return true;
}
