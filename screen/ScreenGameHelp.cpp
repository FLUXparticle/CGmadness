#include "ScreenGameHelp.hpp"

#include "MenuManager.hpp"

#include "utils/Singleton.hpp"

#include "keyboard.hpp"

#include "macros.hpp"

typedef struct
{
	char* left;
	char* right;
} LeftRight;

static LeftRight gTextHelp[] =
{
	{"Cursor", "Move"},
	{"Space", "Jump"},
	{"W A S D", "Camera"},
	{"R F", "Zoom"},
	{"Enter", "Reset"},
	{"Esc", "Menu"},
};

static Singleton<MenuManager> gMenuManager;

static void clickButtonBack()
{
	gMenuManager->popScreen();
}

ScreenGameHelp::ScreenGameHelp() :
	lTextHelp(2 * LENGTH(gTextHelp))
{
	for (unsigned int i = 0; i < lTextHelp.size(); i++)
	{
		int row = i / 2;
		int col = i % 2;
		float z = 6.0f - row;

		lTextHelp[i] = Label(col ? 5.0f : -5.0f, z, 1.0f, col,
							col ? gTextHelp[row].right : gTextHelp[row].left);
	}

	bBack = Button(6.0f - LENGTH(gTextHelp), clickButtonBack, "back", KEY_ESC);
	
	mItems.push_back(&bBack);
}

ScreenGameHelp::~ScreenGameHelp()
{
  // empty
}

