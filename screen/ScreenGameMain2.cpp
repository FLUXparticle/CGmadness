#include "ScreenGameMain2.hpp"

#include "MenuManager.hpp"

#include "utils/Singleton.hpp"

#include "keyboard.hpp"
#include "main.hpp"

static Singleton<MenuManager> gMenuManager;

static void clickButtonResume()
{
	gMenuManager->popScreen();
	resumeGame();
}

ScreenGameMain2::ScreenGameMain2()
{
	bResume = Button(6.0f, clickButtonResume, "resume", KEY_ENTER);
}

ScreenGameMain2::~ScreenGameMain2()
{
  // empty
}

