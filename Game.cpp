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
#include "features.hpp"
#include "keyboard.hpp"
#include "camera.hpp"
#include "texture.hpp"
#include "environment.hpp"
#include "atlas.hpp"
#include "main.hpp"
#include "objects.hpp"

#include "functions.hpp"

#define COUNTDOWN_TIME 1.0f

PlayersBall& Game::sgoBall = PlayersBall::sgoBall;

Game::Game()
{
  gScreenMain1 = new ScreenGameMain1(this);
  gScreenMain2 = new ScreenGameMain2(this);
  
	mTextureRing = loadTexture("data/ring.tga", false);
}

Game::~Game()
{
  // empty
}

void Game::pauseGame()
{
	disableBall();
	disableBallCamera();
	mGameState = STATE_MENU;
}

void Game::resumeGame()
{
	enableBallCamera();
	mGameState = STATE_WAITING;
}

void Game::update(float interval)
{
	updateEnvironment(interval);
	switch (mGameState) {
		case STATE_MENU:
			gMenuManager->update(interval);
			break;
		case STATE_WAITING:
			updateBall(sgoBall, interval);
			
			if ((sgoBall.pos() - sgLookat).len() < 1.0f)
			{
				mCounter = 0.0f;
				mGameState = STATE_COUNTDOWN;
			}
			break;
		case STATE_COUNTDOWN:
			mCounter += interval;
			if (mCounter > COUNTDOWN_TIME)
			{
				enableBall();
				mGameState = STATE_RUNNING;
			}
		case STATE_RUNNING:
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
			break;
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

void Game::draw()
{
	drawEnvironment(this);

	drawGameField(false);
	sgoBall.drawGameBall();

	switch (mGameState) {
	case STATE_MENU:
		gMenuManager->draw();
		break;
	case STATE_COUNTDOWN:
		glPushMatrix();
		{
			const Vector3 x = rotateVector(Vector3(1.0f, 0.0f, 0.0f));
			const Vector3 y = rotateVector(Vector3(0.0f, 1.0f, 0.0f));
			const Vector3 z = rotateVector(Vector3(0.0f, 0.0f, 1.0f));
			
			const Vector3& pos = sgoBall.pos();
			
			Matrix m;
			
			m[0][0] = x.x;
			m[0][1] = x.y;
			m[0][2] = x.z;
			m[0][3] = 0.0f;
			
			m[1][0] = y.x;
			m[1][1] = y.y;
			m[1][2] = y.z;
			m[1][3] = 0.0f;
			
			m[2][0] = z.x;
			m[2][1] = z.y;
			m[2][2] = z.z;
			m[2][3] = 0.0f;
			
			m[3][0] = pos.x;
			m[3][1] = pos.y;
			m[3][2] = pos.z;
			m[3][3] = 1.0f;
			
			glMultMatrixf((GLfloat*) m);
			
			float p = 2.0f * mCounter / COUNTDOWN_TIME;
			
			drawRingStrip(100, max(0.0f, p - 1.0f), min(1.0f, p), mTextureRing);
		}
		glPushMatrix();
		break;
	default:
		break;
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
