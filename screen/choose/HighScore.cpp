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

#include "HighScore.hpp"

#include "LevelLoader.hpp"

#include "utils/Singleton.hpp"

#include "graphics/objects.hpp"
#include "text/text.hpp"

#include "level/level.hpp"
#include "level/io.hpp"

#include "hw/keyboard.hpp"

#include "ColorStack.hpp"

#include <cstdio>
#include <cstring>
#include <cmath>

#define HIGHSCORE_WIDTH 4.0f
#define HIGHSCORE_HEIGHT 4.0f

HighScore::HighScore() :
	Canvas(false)
{
  // empty
}

HighScore::HighScore(float z) :
	Canvas(z, HIGHSCORE_WIDTH, HIGHSCORE_HEIGHT, false)
{
  // empty
}

HighScore::~HighScore()
{
  // empty
}

bool HighScore::acceptHighScoreName()
{
	if (sgLevel.lastPlayerIndex < MAX_SCORE_COLS)
	{
		mLastEntry = sgLevel.scores[sgLevel.lastPlayerIndex].name;

		saveHighscoreToFile();
		sgLevel.lastPlayerIndex = MAX_SCORE_COLS;

		return false;
	}

	return true;
}

void HighScore::update(float interval)
{
	static float time = 0.0f;

	unsigned char ch = getLastChar();

	time += interval;

	gShowCursor = time - floor(time) < 0.5;

	if (sgLevel.lastPlayerIndex < MAX_SCORE_COLS && wasKeyPressed(ch))
	{
		char* name = sgLevel.scores[sgLevel.lastPlayerIndex].name;
		int len = strlen(name);

		switch (ch)
		{
		case '\b':
		case KEY_DELETE:
			if (len > 0)
			{
				name[len - 1] = '\0';
			}
			break;
		case KEY_ENTER:
			acceptHighScoreName();
			break;
		default:
			if (ch >= MIN_ALLOWED_CHAR && ch <= MAX_ALLOWED_CHAR)
			{
				if (len < MAX_NAME_LENGTH)
				{
					name[len] = ch;
					name[len + 1] = '\0';
				}
			}
			else
			{
				printf("not allowed character: %d\n", ch);
			}
		}
	}
}

void HighScore::draw() const
{
	Singleton<LevelLoader> gLevelLoader;

	const char* name = gLevelLoader->name();

	float scale = 0.5f * HIGHSCORE_HEIGHT / (MAX_SCORE_COLS + 2);

	drawPanel(HIGHSCORE_WIDTH, HIGHSCORE_HEIGHT);

	glPushMatrix();
	{
		glTranslatef(HIGHSCORE_WIDTH / 2.0f,
				(float) (MAX_SCORE_COLS + 1) / (MAX_SCORE_COLS +
						2) * HIGHSCORE_HEIGHT, 0.0f);

		glScalef(scale, scale, scale);

		glTranslatef(-widthStrokeText(name) / 2.0f, 0.0f, 0.0f);

		ColorStack::colorStack.setColor(Color4::blue);

		drawStrokeThinText(name);
	}
	glPopMatrix();

	for (int i = 0; i < sgLevel.cntScoreCols; i++)
	{
		char strName[MAX_NAME_LENGTH + 4];
		char strTime[10];

		int tenthSecond = sgLevel.scores[i].tenthSecond;

		sprintf(strName, "%2d%c%s", i + 1, 0, sgLevel.scores[i].name);
		sprintf(strTime, "%d:%02d.%01d", tenthSecond / 600, tenthSecond / 10 % 60,
				tenthSecond % 10);

		if (i == sgLevel.lastPlayerIndex)
		{
			ColorStack::colorStack.setColor(Color4::red);
		}
		else
		{
			ColorStack::colorStack.setColor(Color4::white);
		}

		glPushMatrix();
		{
			glTranslatef(0.0f,
					(float) (MAX_SCORE_COLS - i) / (MAX_SCORE_COLS +
							2) * HIGHSCORE_HEIGHT, 0.0f);

			glScalef(scale, scale, scale);

			glPushMatrix();
			{
				glTranslatef((0.1f * HIGHSCORE_WIDTH / scale) - widthStrokeText(strName),
						0.0f, 0.0f);

				strName[2] = ' ';

				drawStrokeThinText(strName);

				glTranslatef(widthStrokeText(strName), 0.0f, 0.0f);

				if (gShowCursor && i == sgLevel.lastPlayerIndex)
				{
					drawStrokeThinText("_");
				}
			}
			glPopMatrix();

			glTranslatef((0.95f * HIGHSCORE_WIDTH / scale) - widthStrokeText(strTime),
					0.0f, 0.0f);

			drawStrokeThinText(strTime);
		}
		glPopMatrix();
	}

	ColorStack::colorStack.setColor(Color4::white);
}
