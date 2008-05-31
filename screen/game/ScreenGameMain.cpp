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

#include "ScreenGameMain.hpp"

#include "process/MainProcess.hpp"
#include "ScreenGameHelp.hpp"

#include "ball/PlayersBall.hpp"

#include "utils/Callback.hpp"

#include "Main.hpp"

#include "hw/features.hpp"
#include "hw/keyboard.hpp"
#include "Main.hpp"

static void changeBallEdit(const void *self)
{
	const SpinEdit* spinedit = (const SpinEdit*) self;
	PlayersBall::sgoBall.layout() = spinedit->value;
}

static void changeBallShadow(const void *self)
{
	const Check *check = (const Check *) self;
	setBallShadow(check->value);
}

static void changeReflection(const void *self)
{
	const Check *check = (const Check *) self;
	setReflection(check->value);
}

static void drawMenuBall()
{
	PlayersBall::sgoBall.drawMenuBall();
}

ScreenGameMain::ScreenGameMain()
{
	int maxLayout = PlayersBall::sgoBall.cntLayouts() - 1;
	
	gseBall = SpinEdit(maxLayout, 0, maxLayout, 4.3, 5.2f, drawMenuBall, changeBallEdit);
	mItems.push_back(&gseBall);

	gcBallShadow = Check(4.0f, changeBallShadow, "ball shadow");
	mItems.push_back(&gcBallShadow);
	
	gcReflection = Check(3.0f, changeReflection, "reflection");
	mItems.push_back(&gcReflection);

	bHelp = Button(2.0f, CALLBACK(ScreenGameMain, clickButtonHelp), "help", 'h');
	mItems.push_back(&bHelp);
	
	bQuit = Button(1.0f, CALLBACK(ScreenGameMain, clickButtonQuit), "give up", KEY_ESC);
	mItems.push_back(&bQuit);
}

ScreenGameMain::~ScreenGameMain()
{
  // empty
}

void ScreenGameMain::show()
{
	Screen::show();
	
	gcBallShadow.set(useBallShadow());
	gcReflection.set(useReflection());
	gseBall.value = PlayersBall::sgoBall.layout();
}

void ScreenGameMain::clickButtonHelp()
{
	Main::setState(gScreenHelp);
}

void ScreenGameMain::clickButtonQuit()
{
	Main::popScreenStatic();
	
	Singleton<MainProcess> main;
	Main::setState(main);
}
