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

#include "RaceTheClock.hpp"

#include "screen/game/ScreenGameEnd.hpp"

#include "MenuManager.hpp"

#include "level.hpp"
#include "highscore.hpp"
#include "text/text.hpp"

#include "functions.hpp"

#include <GL/gl.h>

#include <stdio.h>

RaceTheClock::RaceTheClock()
{
	gScreenEnd = new ScreenGameEnd(this);
}

RaceTheClock::~RaceTheClock()
{
  // empty
}

void RaceTheClock::resetGame()
{
	Game::resetGame();
	resetGameTime();
}

void RaceTheClock::update(float interval)
{
	bool wasInPieces = sgoBall.isInPieces();
	
	Game::update(interval);
	
	if (gIsGameRunning)
	{
		if (!sgoBall.isInPieces())
		{
			if (wasInPieces)
			{
				resetGameTime();
			}
			
			gGameTime += interval;
		}

		if (sgoBall.hasHitGoal())
		{
			finishedGame();
		}
	}
}

void RaceTheClock::drawHUD(float widthWindow, float heightWindow)
{
	int tenthSecond = (int) (gGameTime * 10.0f);
	float scale = 0.06f;
	float widthDefault = widthStrokeText("x:xx.x") * scale;

	char strTime[10];
	float width;
	float height;

	sprintf(strTime, "%d:%02d.%01d", tenthSecond / 600, tenthSecond / 10 % 60,
					tenthSecond % 10);

	width = widthStrokeText(strTime) * scale;
	height = scale;

	glColor3f(1.0f, 1.0f, 0.0f);

	glPushMatrix();

	glTranslatef((widthWindow - widthDefault) / 2.0f, (heightWindow - height),
							 0.0f);
	glScalef(scale, scale, scale);

	drawStrokeThickText(strTime);

	glPopMatrix();
}

void RaceTheClock::resetGameTime()
{
	gGameTime = 0.0f;
}

void RaceTheClock::stopWatch()
{
	int i;

	int tenthSecond = (int) (gGameTime * 10.0f);
	int newIndex = sgLevel.cntScoreCols;

	while (newIndex > 0 && tenthSecond < sgLevel.scores[newIndex - 1].tenthSecond)
	{
		newIndex--;
	}

	sgLevel.cntScoreCols = min(sgLevel.cntScoreCols + 1, MAX_SCORE_COLS);

	for (i = sgLevel.cntScoreCols - 1; i > newIndex; i--)
	{
		sgLevel.scores[i] = sgLevel.scores[i - 1];
	}

	if (newIndex < MAX_SCORE_COLS)
	{
		sgLevel.scores[newIndex].name[0] = '\0';
		sgLevel.scores[newIndex].tenthSecond = tenthSecond;
	}

	sgLastPlayerIndex = newIndex;
}

void RaceTheClock::finishedGame()
{
	stopWatch();
	pauseGame();
	gMenuManager->pushScreen(gScreenEnd);
}
