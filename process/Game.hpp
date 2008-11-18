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

#ifndef Game_hpp
#define Game_hpp

#include "Process.hpp"
#include "reflection/WaterReflection.hpp"
#include "reflection/BallReflection.hpp"

#include "ball/PlayersBall.hpp"

#include "utils/SmartPointer.hpp"

class Game : public Process, public WaterReflection, public BallReflection
{
public:
	typedef enum
	{
		STATE_MENU,
		STATE_WAITING,
		STATE_COUNTDOWN,
		STATE_RUNNING
	} GameState;

public:
  Game();
  virtual ~Game();

  void start(Process* previous, bool push);
  void stop();

  void suspend();
  void resume();

  void update(float interval);

  void preDisplay();
  void draw() const;

  virtual void resetGame();
  void leaveGame();

	void drawWaterReflection() const;
	void drawBallReflection() const;

protected:
  static PlayersBall& sgoBall;

protected:
	GameState mGameState;

	float mCounter;

	Process* mPrevious;

	SmartPointer<class ScreenGameMain1> gScreenMain1;
	SmartPointer<class ScreenGameMain2> gScreenMain2;

  void pauseGame();

private:
	int mTextureRing;
};

#endif
