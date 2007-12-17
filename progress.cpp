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

#include "progress.hpp"

#include <stdio.h>
#include <time.h>

static float gCurProgress;
static time_t gLastUpdate;

void resetProgress(void)
{
	gCurProgress = 0.0f;
	gLastUpdate = time(NULL);
}

void setProgress(float progress)
{
	time_t curTime = time(NULL);
	if (curTime > gLastUpdate && progress > gCurProgress)
	{
		gCurProgress = progress;
		gLastUpdate = curTime;

		printf("%.1f%%\n", gCurProgress * 100.0f);
	}

}
