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

#ifndef PlayersBall_hpp
#define PlayersBall_hpp

#include "Ball.hpp"

#define MAX_BALL_LAYOUTS 5

#define BALL_LAYOUT_DEFAULT 0
#define BALL_LAYOUT_TEXTURE 1
#define BALL_LAYOUT_METAL 2
#define BALL_LAYOUT_GOLFBALL 3
#define BALL_LAYOUT_GOLFBALL_METAL 4

class PlayersBall : public Ball
{
public:
	static PlayersBall sgoBall;
	
	static void init();
	
public:
	void initCubeMap();
	
	bool hasCubeMap() const;
	bool useBallReflection() const;
	
  void changeBall(int layout);

  void update(float interval);

  void updateReflection();
	void drawMenuBall() const;

private:
	int mCubeMapBall;
	bool mIsReflectionDirty;
	
	int mBallLayout;

	RenderTarget mTargetCube[6];
	Viewport mViewportCube[6];
	
  PlayersBall();
  virtual ~PlayersBall();

  void drawBall() const;
  void drawExplosion() const;
  
  void activateBallShader() const;
  void deactivateBallShader() const;
  
	bool useBallShader() const;

};

void drawMenuBall(void);

#endif
