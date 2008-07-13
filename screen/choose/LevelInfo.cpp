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

#include "LevelInfo.hpp"

#include "LevelLoader.hpp"

#include "utils/Singleton.hpp"

#include "level.hpp"
#include "objects.hpp"
#include "text/text.hpp"

#include "ColorStack.hpp"

#include <stdio.h>
#include <GL/gl.h>

#define LEVELINFO_WIDTH 4.0f
#define LEVELINFO_HEIGHT 4.0f
#define LEVELINFO_LINES 11

LevelInfo::LevelInfo()
{
  // empty
}

LevelInfo::LevelInfo(float z) :
	Canvas(z, LEVELINFO_WIDTH, LEVELINFO_HEIGHT)
{
  // empty
}

LevelInfo::~LevelInfo()
{
  // empty
}

void LevelInfo::draw() const
{
	Singleton<LevelLoader> gLevelLoader;

	const char *lines[LEVELINFO_LINES];
	char size[20];
	int i;

	float scale = 0.5f * LEVELINFO_HEIGHT / (LEVELINFO_LINES + 1);

	sprintf(size, "size: %d x %d", sgLevel.size.x, sgLevel.size.y);

	lines[0] = gLevelLoader->name();
	lines[1] = "";
	lines[2] = "";
	lines[3] = "";
	lines[4] = "";
	lines[5] = size;
	lines[6] = "";
	lines[7] = "";
	lines[8] = "";
	lines[9] = "";
	lines[10] = "";

	drawPanel(LEVELINFO_WIDTH, LEVELINFO_HEIGHT);

	for (i = 0; i < LEVELINFO_LINES; i++)
	{
		glPushMatrix();

		glTranslatef(LEVELINFO_WIDTH / 2.0f,
								 (float) (LEVELINFO_LINES - i) / (LEVELINFO_LINES +
																									1) * LEVELINFO_HEIGHT, 0.0f);

		glScalef(scale, scale, scale);

		glTranslatef(-widthStrokeText(lines[i]) / 2.0f, 0.0f, 0.0f);

		switch (i)
		{
		case 0:
			ColorStack::colorStack.setColor(Color4::blue);
			break;
		case 5:
			ColorStack::colorStack.setColor(Color4::foo);
			break;
		default:
			ColorStack::colorStack.setColor(Color4::white);
			break;
		}

		drawStrokeThinText(lines[i]);

		glPopMatrix();
	}

	ColorStack::colorStack.setColor(Color4::white);
}
