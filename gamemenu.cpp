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

#include "gamemenu.hpp"

#include "screen/ScreenGameMain1.hpp"
#include "screen/ScreenGameMain2.hpp"
#include "screen/ScreenGameHelp.hpp"
#include "screen/ScreenGameEnd.hpp"

#include "gui/Label.hpp"
#include "gui/Button.hpp"
#include "gui/Check.hpp"
#include "gui/SpinEdit.hpp"

#include "MenuManager.hpp"

#include "utils/Singleton.hpp"

#include "PlayersBall.hpp"
#include "text.hpp"
#include "level.hpp"
#include "Game.hpp"
#include "features.hpp"
#include "keyboard.hpp"
#include "highscore.hpp"
#include "main.hpp"
#include "objects.hpp"

#include <stdio.h>
#include <string.h>

static Singleton<ScreenGameMain1> gScreenMain1;
static Singleton<ScreenGameMain2> gScreenMain2;
static Singleton<ScreenGameEnd> gScreenEnd;

static Singleton<MenuManager> gMenuManager;

/* events */

void showGameMenu(int menu)
{
	static Screen* screens[] = {
		gScreenMain1,
		gScreenMain2,
		gScreenEnd
	};

	gMenuManager->pushScreen(screens[menu]);
	
	screens[menu]->show();
}

void initGameMenu(void)
{
	// TODO
}
