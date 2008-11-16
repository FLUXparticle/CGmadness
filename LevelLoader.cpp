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

#include "LevelLoader.hpp"

#include "level/io.hpp"
#include "level/level.hpp"

#include "common.hpp"

#include "texture.hpp"

#include <string.h>

LevelLoader::LevelLoader() :
	gLoadedLevel(-1)
{
	createStringListFromDir(&sgLevels, "levels");
}

LevelLoader::~LevelLoader()
{
  // empty
}

int LevelLoader::maxID() const
{
	return sgLevels.count - 1;
}

void LevelLoader::loadLevelByID(int id)
{
	if (id != gLoadedLevel)
	{
		if (gLoadedLevel >= 0)
		{
			destroyLevel();
			gLoadedLevel = -1;
		}

		if (id >= 0 && loadLevelFromFile(sgLevels.strings[id]))
		{
			gLoadedLevel = id;

			if (sgLevel.borderTexture == 0)
			{
				sgLevel.borderTexture = loadTexture("data/plate.tga", true);
			}

			sgCurLevelname =
				sgLevels.strings[gLoadedLevel] +
				strlen(sgLevels.strings[gLoadedLevel]) + 1;
		}
	}
}
