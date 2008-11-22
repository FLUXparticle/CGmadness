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

#include "file.hpp"
#include "crc32.hpp"
#include "level.hpp"

#include "kdtree/KdPaintersAlgorithmReverse.hpp"
#include "kdtree/KdList.hpp"

#include "atlas.hpp"
#include "common.hpp"

#include "utils/String.hpp"

#include "functions.hpp"

#include "macros.hpp"

#include <list>

#define EXT_HIGHSCORE ".highscore"

#define THIS_CGM_VERSION 4
#define THIS_HIGHSCORE_VERSION 1

#define MAX_LEVEL_SIZE 100

static void toLightMap(int index, bool flip, int dataInt[SIZEOF_LIGHT_MAP])
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

bool loadHighscoreFromFile()
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

static void importLightmapV2(FILE* file)
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
			toLightMap(index++, false, tmp[y * sgLevel.size.x + x]);

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
						toLightMap(index++, (side / 2) != 0, tmp[indexTmp]);
					}
				}
				else
				{
					for (int j = bottom; j < top; j++)
					{
						int indexTmp = starts[side] + x * 2 * sgLevel.size.y * (MAX_LEVEL_HEIGHT + 1) + j * sgLevel.size.y + y;
						toLightMap(index++, (side / 2) != 0, tmp[indexTmp]);
					}
				}
			}
		}
	}

	delete[] tmp;
}

static void newLevel()
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
			KdCell::Range& range = sgLevel.kdLevelTree->get(x, y);

			{
				Block b;

				b.x = x;
				b.y = y;
				b.z = 0;
				b.dzx = 0;
				b.dzy = 0;
				b.dirty = true;

				range.start = sgLevel.blocks.insert(b);
			}
			range.end = range.start + 1;
		}
	}

	initCommon();

	updateLightMap(false);
}

bool loadLevelFromFile(const char* filename)
{
	FILE* file = fopen(filename, "rt");

	unsigned int version;
	unsigned int crc32;

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
	readFieldCoord(file, sgLevel.start);
	readFieldCoord(file, sgLevel.finish);
	readFieldCoord(file, sgLevel.size);

	if (version >= 4)
	{
		readString(file, sgLevel.author);
	}

	initLevel();

	/* reading data */
	if (version >= 4)
	{
		int countBlocks = readInt(file);

		for (int i = 0; i < countBlocks; ++i)
		{
			KdCell::Range range;
			int x;
			int y;

			{
				Block b;

				readFieldBlock(file, b);
				b.dirty = true;

				x = b.x;
				y = b.y;

				range.start = sgLevel.blocks.insert(b);
			}
			range.end = range.start + 1;

			sgLevel.kdLevelTree->get(x, y) = range;
		}
	}
	else
	{
		for (int x = 0; x < sgLevel.size.x; x++)
		{
			for (int y = 0; y < sgLevel.size.y; y++)
			{
				KdCell::Range& range = sgLevel.kdLevelTree->get(x, y);

				{
					Block b;

					b.x = x;
					b.y = y;
					readFieldBlockV3(file, b);
					b.dirty = true;

					range.start = sgLevel.blocks.insert(b);
				}
				range.end = range.start + 1;
			}
		}
	}

	sgLevel.crc32 = getCRC32();

	initCommon();

	if (version >= 2)
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

					toLightMap(index, false, dataInt);

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
		updateLightMap(false);
	}

	fclose(file);

	if (!loadHighscoreFromFile())
	{
		sgLevel.cntScoreCols = 0;
	}

	sgLevel.saved = true;

	return true;
}

bool saveHighscoreToFile()
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

bool saveLevelToFile(bool shrink)
{
	FILE *file = fopen(sgLevel.filename, "wt");

	if (!file)
	{
		return false;
	}

	FieldCoord fcMin;
	FieldCoord fcMax;

	std::list<int> indices;
	{
		Vector3 origin(0.0f, 0.0f, 0.0f);
		KdPaintersAlgorithmReverse iter(*sgLevel.kdLevelTree, origin);
		KdList list(iter);

		if (list.next())
		{
			const Block& b = sgLevel.blocks[*list];

			fcMin.x = b.x;
			fcMin.y = b.y;
			fcMax.x = b.x;
			fcMax.y = b.y;
		}
		else
		{
			fcMin.x = 0;
			fcMin.y = 0;
			fcMax.x = 0;
			fcMax.y = 0;
		}

		do
		{
			int index = *list;
			const Block& b = sgLevel.blocks[index];

			fcMin.x = min(fcMin.x, b.x);
			fcMin.y = min(fcMin.y, b.y);
			fcMax.x = max(fcMax.x, b.x);
			fcMax.y = max(fcMax.y, b.y);

			indices.push_back(index);
		} while (list.next());
	}

	/* version number */
	fprintf(file, "v%u\n", THIS_CGM_VERSION);

	resetCRC32();

	if (shrink)
	{
		sgLevel.start.x -= fcMin.x;
		sgLevel.start.y -= fcMin.y;
		sgLevel.finish.x -= fcMin.x;
		sgLevel.finish.y -= fcMin.y;
		sgLevel.size.x = fcMax.x - fcMin.x + 1;
		sgLevel.size.y = fcMax.y - fcMin.y + 1;
	}

	/* write atributes */
	writeFieldCoord(file, sgLevel.start);
	writeFieldCoord(file, sgLevel.finish);
	writeFieldCoord(file, sgLevel.size);

	writeString(file, sgLevel.author);
	fputc('\n', file);

	/* write data */
	writeInt(file, indices.size());
	fputc('\n', file);

	FOREACH(indices, iter)
	{
		Block b = sgLevel.blocks[*iter];

		if (shrink)
		{
			b.x -= fcMin.x;
			b.y -= fcMin.y;
		}

		writeFieldBlock(file, b);
	}

	fprintf(file, "%08X\n", getCRC32());

	FOREACH(indices, iter)
	{
		Range subatlas = sgLevel.blocks[*iter].subatlas;

		for (int i = subatlas.start; i < subatlas.end; i++)
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
	}

	fprintf(file, "%08X\n", getCRC32());

	if (fclose(file) != 0)
	{
		return false;
	}

	return true;
}
