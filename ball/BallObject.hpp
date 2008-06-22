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

#ifndef BallObject_hpp
#define BallObject_hpp

#include "math/Vector3.hpp"

#define SUB_DIVS_DEPTH 3

/*
 * WARNING: CNT_BALL_TRIANGLES must be (20 * (4 ^ SUB_DIVS_DEPTH))
 */

#define CNT_BALL_TRIANGLES (20 * 64)
#define CNT_BALL_VERTICES (CNT_BALL_TRIANGLES * 3)

class BallObject
{
public:
	static void init();
	
public:
  BallObject();
  virtual ~BallObject();
  
  void drawDefault() const;
  void drawShader() const;
  
protected:
	static Vector3 gBallVertices[CNT_BALL_VERTICES];
	static Vector3 gBallTexCoordsDefault[CNT_BALL_VERTICES];
	static Vector3 gBallTexCoordsShader[CNT_BALL_VERTICES];

protected:
	void draw(Vector3 ballTexCoords[CNT_BALL_VERTICES]) const;
	
private:
	static int gCntBallVertices;

	static void subdiv(int depth, Vector3 tri[3], Vector3 hole, float s);
	
};

#endif
