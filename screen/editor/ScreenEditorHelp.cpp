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

#include "ScreenEditorHelp.hpp"

#include "macros.hpp"

static LeftRight gTextHelp[] =
{
	{"Cursor", "move"},
	{"Shift", "multi select"},
	{"W A S D", "pitch"},
	{"R F", "hight"},
	{"0", "flatten"},
	{"+/-", "add/remove blocks"},
	{"Enter", "test"},
	{"Y X C V B N", "camera"},
	{"1 2", "start finish"},
	{"Esc", "menu"},
};

ScreenEditorHelp::ScreenEditorHelp() :
	ScreenHelp(LENGTH(gTextHelp), gTextHelp, 0.75f)
{
  // empty
}

ScreenEditorHelp::~ScreenEditorHelp()
{
  // empty
}

