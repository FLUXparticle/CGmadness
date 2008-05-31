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

#include "Game.hpp"

#include "screen/game/ScreenGameMain1.hpp"
#include "screen/game/ScreenGameMain2.hpp"

#include "MenuManager.hpp"

#include "common.hpp"
#include "level.hpp"

#include "ballcamera.hpp"
#include "field.hpp"
#include "hw/features.hpp"
#include "hw/keyboard.hpp"
#include "camera.hpp"
#include "texture.hpp"
#include "environment/environment.hpp"
#include "atlas.hpp"
#include "Main.hpp"

#include "functions.hpp"

PlayersBall& Game::sgoBall = PlayersBall::sgoBall;

Game::Game()
{
  gScreenMain1 = new ScreenGameMain1(this);
  gScreenMain2 = new ScreenGameMain2(this);
}

Game::~Game()
{
  // empty
}

void Game::pauseGame()
{
	disableBallCamera();
	gIsGameRunning = false;
}

void Game::resumeGame()
{
	enableBallCamera();
	gIsGameRunning = true;
}

void Game::update(float interval)
{
	updateEnvironment(interval);
	if (gIsGameRunning)
	{
		if (wasKeyPressed(KEY_ESC))
		{
			pauseGame();
			gMenuManager->pushScreen(gScreenMain2);
		}

		/* manually switch features */
		if (wasFunctionPressed(1))
		{
			setBallShadow(!useBallShadow());
		}

		if (wasFunctionPressed(2))
		{
			setReflection(!useReflection());
		}

		if (wasFunctionPressed(5))
		{
			pauseGame();
			toggleMouseControl();
			resumeGame();
		}

		updateBall(sgoBall, interval);
	}
	else
	{
		gMenuManager->update(interval);
	}

	updateGameField(sgoBall);
}

void Game::preDisplay()
{
	sgoBall.updateReflection(this);
}

void Game::drawWaterReflection() const
{
	drawGameField(false);
	sgoBall.drawGameBall();
}

void Game::drawBallReflection() const
{
	drawEnvironment(this);
	drawGameField(true);
}

void Game::draw() const
{
	drawEnvironment(this);

	drawGameField(false);
	sgoBall.drawGameBall();

	if (!gIsGameRunning)
	{
		gMenuManager->draw();
	}
}

void lightMapToTexture(unsigned int texID)
{
	unsigned int sizeX;
	unsigned int sizeY;
	const float* data;
	
	getAtlasInfo(&sizeX, &sizeY, &data);
	
	glBindTexture(GL_TEXTURE_2D, texID);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_LUMINANCE8, sizeX, sizeY, 0, GL_LUMINANCE, GL_FLOAT, data);
}

void Game::start()
{
	sgLevel.lightMap = genTexture();
	lightMapToTexture(sgLevel.lightMap);

	updateTexCoords();

	initGameField();

	resetGame();
}

void Game::stop(void)
{
	glDeleteTextures(1, &sgLevel.lightMap);

	destroyGameField();
}

void Game::resetGame()
{
	sgoBall.reset();
	resetBallCamera();

	updateGameField(sgoBall);

	pauseGame();
	gMenuManager->pushScreen(gScreenMain1);
}
