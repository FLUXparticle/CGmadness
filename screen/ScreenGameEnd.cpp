#include "ScreenGameEnd.hpp"

#include "MenuManager.hpp"

#include "utils/Singleton.hpp"

#include "keyboard.hpp"
#include "main.hpp"

static Singleton<MenuManager> gMenuManager;

static void clickButtonAgain()
{
	acceptHighScoreName();
	gMenuManager->popScreen();
	resetGame();
}

static void clickButtonQuit2()
{
	acceptHighScoreName();
	gMenuManager->popScreen();
	setMainState(STATE_MAIN);
}

ScreenGameEnd::ScreenGameEnd()
{
	initHighScore(&hsHighScore, 3.0f);

	bAgain = Button(2.0f, clickButtonAgain, "play again", KEY_ENTER);
	bQuit2 = Button(1.0f, clickButtonQuit2, "change level", KEY_ESC);
}

ScreenGameEnd::~ScreenGameEnd()
{
  // empty
}

