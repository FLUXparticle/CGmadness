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

#ifndef Ball_hpp
#define Ball_hpp

#include "callback.hpp"

#include "BallObject.hpp"
#include "Explosion.hpp"

#include "math/quaternion.hpp"
#include "math/Vector3.hpp"

#define BALL_RADIUS 0.2f

class Ball
{
public:
  static void init();

	static bool hasBallTexture();

public:
  Ball();
  virtual ~Ball();

  const Vector3& pos() const;
  bool hasHitGoal() const;
  bool isInPieces() const;

  virtual void reset();

  void push(const Vector3& direction);
  void jump();
  virtual void update(float interval);

  void drawGameBall() const;

	void explodeBall();

protected:
	static int sTextureBall;

protected:
	const float mMass;
	const float mRadius;

	Vector3 mPos;
	Vector3 mVelocity;

	Quaternion mOrientation;
	Vector3 mAngularRate;

	bool mIsBallInPieces;
	bool mHasBallHitGoal;

	Vector3 mPushDirection;
	bool mJump;

	BallObject mBallObject;
	Explosion mExplosion;

  virtual void drawBall() const;
  virtual void drawExplosion() const;
	void animateBall(float interval);

  virtual void activateBallShader() const;
  virtual void deactivateBallShader() const;

};

inline const Vector3& Ball::pos() const
{
	return mPos;
}

inline bool Ball::hasHitGoal() const
{
	return mHasBallHitGoal;
}

inline bool Ball::isInPieces() const
{
	return mIsBallInPieces;
}

#endif
