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

#include "path/NoiseWorld.hpp"

#include "level/io.hpp"
#include "level/crc32.hpp"
#include "level/level.hpp"

#include "common.hpp"

#include <cstring>

int main(int argc, char* argv[])
{
	NoiseWorld noiseworld;
	World& world = noiseworld;

	sgLevel.filename = "levels/00_noise.cgm";

	/* read attributes */
	sgLevel.size.x = WORLD_SIZE_X;
	sgLevel.size.y = WORLD_SIZE_Y;

	strcpy(sgLevel.author, "noise");

	initLevel();

	bool first = true;
	for (int y = 0; y < WORLD_SIZE_Y; y++)
	{
		for (int x = 0; x < WORLD_SIZE_X; x++)
		{
			int height = world.getHeight(x, y);
			if (height > 0)
			{
				if (first)
				{
					sgLevel.start.x = x;
					sgLevel.start.y = y;
					first = false;
				}

				sgLevel.finish.x = x;
				sgLevel.finish.y = y;

				KdCell::Range range;

				{
					Block b;

					b.x = x;
					b.y = y;
					b.z = height;
					b.dzx = 0;
					b.dzy = 0;
					b.dirty = true;

					range.start = sgLevel.blocks.insert(b);
				}
				range.end = range.start + 1;

				sgLevel.kdLevelTree->get(x, y) = range;
			}
		}
	}

	sgLevel.crc32 = getCRC32();

	initCommon();

	updateLightMap();

	sgLevel.cntScoreCols = 0;

	sgLevel.saved = false;

	saveLevelToFile(true);

	return 0;
}
