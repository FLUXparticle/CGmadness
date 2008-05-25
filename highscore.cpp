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

#include "highscore.hpp"

#include "LevelLoader.hpp"
#include "utils/Singleton.hpp"

#include "level.hpp"
#include "objects.hpp"

#include "hw/keyboard.hpp"
#include "text/text.hpp"

#include <GL/gl.h>

#include <stdio.h>
#include <string.h>
#include <math.h>

#define HIGHSCORE_WIDTH 4.0f
#define HIGHSCORE_HEIGHT 4.0f

int sgLastPlayerIndex;

static int gShowCursor;

void acceptHighScoreName(void)
{
	if (sgLastPlayerIndex < MAX_SCORE_COLS)
	{
		saveHighscoreToFile();
		sgLastPlayerIndex = MAX_SCORE_COLS;
	}
}

void updateHighScore(float interval)
{
	static float time = 0.0f;

	unsigned char ch = getLastChar();

	time += interval;

	gShowCursor = time - floor(time) < 0.5;

	if (sgLastPlayerIndex < MAX_SCORE_COLS && wasKeyPressed(ch))
	{
		char *name = sgLevel.scores[sgLastPlayerIndex].name;
		int len = strlen(name);

		switch (ch)
		{
		case '\b':
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
		}
	}
}

void drawPanel(float width, float height)
{
	glPushMatrix();

	glTranslatef(width / 2.0f, height / 2.0f, -0.1f);

	glScalef(width / 2.0f, height / 2.0f, 1.0f);

	glColor4f(0.0f, 0.0f, 0.0f, 0.5);

	glEnable(GL_BLEND);

	drawSquare();

	glDisable(GL_BLEND);

	glPopMatrix();
}

void drawHighScore()
{
	Singleton<LevelLoader> gLevelLoader;
	
	const char* name = gLevelLoader->name();

	float scale = 0.5f * HIGHSCORE_HEIGHT / (MAX_SCORE_COLS + 2);

	drawPanel(HIGHSCORE_WIDTH, HIGHSCORE_HEIGHT);

	glPushMatrix();

	glTranslatef(HIGHSCORE_WIDTH / 2.0f,
							 (float) (MAX_SCORE_COLS + 1) / (MAX_SCORE_COLS +
																							 2) * HIGHSCORE_HEIGHT, 0.0f);

	glScalef(scale, scale, scale);

	glTranslatef(-widthStrokeText(name) / 2.0f, 0.0f, 0.0f);

	glColor3f(0.0f, 0.0f, 1.0f);

	drawStrokeThinText(name);

	glPopMatrix();

	for (int i = 0; i < sgLevel.cntScoreCols; i++)
	{
		char strName[MAX_NAME_LENGTH + 4];
		char strTime[10];

		int tenthSecond = sgLevel.scores[i].tenthSecond;

		sprintf(strName, "%2d%c%s", i + 1, 0, sgLevel.scores[i].name);
		sprintf(strTime, "%d:%02d.%01d", tenthSecond / 600, tenthSecond / 10 % 60,
						tenthSecond % 10);

		if (i == sgLastPlayerIndex)
		{
			glColor3f(1.0f, 0.0f, 0.0f);
		}
		else
		{
			glColor3f(1.0f, 1.0f, 1.0f);
		}

		glPushMatrix();

		glTranslatef(0.0f,
								 (float) (MAX_SCORE_COLS - i) / (MAX_SCORE_COLS +
																								 2) * HIGHSCORE_HEIGHT, 0.0f);

		glScalef(scale, scale, scale);

		glPushMatrix();

		glTranslatef((0.1f * HIGHSCORE_WIDTH / scale) - widthStrokeText(strName),
								 0.0f, 0.0f);

		strName[2] = ' ';

		drawStrokeThinText(strName);

		glTranslatef(widthStrokeText(strName), 0.0f, 0.0f);

		if (gShowCursor && i == sgLastPlayerIndex)
		{
			drawStrokeThinText("_");
		}

		glPopMatrix();

		glTranslatef((0.95f * HIGHSCORE_WIDTH / scale) - widthStrokeText(strTime),
								 0.0f, 0.0f);

		drawStrokeThinText(strTime);

		glPopMatrix();
	}

	glColor3f(1.0f, 1.0f, 1.0f);
}

void initHighScore(HighScore* highScore, float z)
{
	*highScore = Canvas(z, HIGHSCORE_WIDTH, HIGHSCORE_HEIGHT, updateHighScore, drawHighScore);
}
