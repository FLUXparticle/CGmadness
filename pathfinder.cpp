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

#include <cstdio>

int main(int argc, char* argv[])
{
	NoiseWorld noiseworld;
	World& world = noiseworld;

	for (int y = 0; y < WORLD_SIZE_Y; y++)
	{
		for (int x = 0; x < WORLD_SIZE_X; x++)
		{
			int height = world.getHeight(x, y);
			if (height > 0)
			{
				printf("%3d", height);
			}
			else
			{
				printf("   ");
			}
		}
		printf("\n");
	}

	return 0;
}
