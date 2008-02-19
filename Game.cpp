#include "Game.hpp"

#include "common.hpp"
#include "level.hpp"

#include "ballcamera.hpp"
#include "field.hpp"
#include "menumanager.hpp"
#include "gamemenu.hpp"
#include "features.hpp"
#include "keyboard.hpp"
#include "camera.hpp"
#include "texture.hpp"
#include "environment.hpp"
#include "atlas.hpp"
#include "main.hpp"

#include "functions.hpp"

PlayersBall& Game::sgoBall = PlayersBall::sgoBall;

Game::Game()
{
  // empty
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
			showGameMenu(1);
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
		updateMenuManager(interval);
	}

	updateGameField(sgoBall);
}

void Game::preDisplay()
{
	sgoBall.updateReflection();
}

void Game::drawWaterReflection() const
{
	drawGameField(false);
	sgoBall.drawGameBall();
}

void Game::drawGameBallReflection() const
{
	drawEnvironment(drawGameWaterReflection);
	drawGameField(true);
}

void Game::draw()
{
	drawEnvironment(drawGameWaterReflection);

	drawGameField(false);
	sgoBall.drawGameBall();

	if (!gIsGameRunning)
	{
		drawMenuManager();
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
	showGameMenu(0);
}

void Game::init()
{
	/* ball */
	PlayersBall::init();

	/* menu (must be after ball) */
	initGameMenu();
}
