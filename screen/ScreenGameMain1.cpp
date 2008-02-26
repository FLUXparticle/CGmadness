#include "ScreenGameMain1.hpp"

#include "MenuManager.hpp"

#include "utils/Singleton.hpp"

#include "keyboard.hpp"
#include "main.hpp"

static Singleton<MenuManager> gMenuManager;

static void clickButtonStart()
{
	gMenuManager->popScreen();
	resumeGame();
}

ScreenGameMain1::ScreenGameMain1()
{
	bStart = Button(6.0f, clickButtonStart, "start", KEY_ENTER);
}

ScreenGameMain1::~ScreenGameMain1()
{
  // empty
}

