/*
 * CG Madness - a Marble Madness clone
 * Copyright (C) 2007  Sven Reinck <sreinck@gmx.de>
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

#include "main.h"

#include "mainmenu.h"
#include "game.h"
#include "editor.h"

typedef enum
{
	STATE_MAIN,
	STATE_GAME,
	STATE_EDITOR
} State;

State gCurState = STATE_MAIN; 

void initMain(void)
{
	// TODO
}

void updateMain(float interval)
{
	switch (gCurState)
	{
	case STATE_MAIN:
		updateMainMenu(interval);
		break;
	case STATE_GAME:
		updateGame(interval);
		break;
	case STATE_EDITOR:
		updateEditor(interval);
		break;
	}
}

