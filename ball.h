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

#ifndef _ball_h_
#define _ball_h_

#include "quaternion.h"

#define MAX_BALL_LAYOUTS 5

#define BALL_LAYOUT_DEFAULT 0
#define BALL_LAYOUT_TEXTURE 1
#define BALL_LAYOUT_METAL 2
#define BALL_LAYOUT_GOLFBALL 3
#define BALL_LAYOUT_GOLFBALL_METAL 4

typedef struct {
	float mass;
	float radius;

	Vector3 pos;
	Vector3 velocity;

	Quaternion orientation;
	Vector3 angularRate;
} Ball;

extern Ball sgoBall;
extern int sgIsBallInPieces;
extern int sgHasBallHitGoal;
extern int sgIsMouseControl;

extern Vector3 sgForward;
extern Vector3 sgRight;

void initBall(void);

void resetBall(void);
void resetBallCamera(void);

void enableBallCamera(void);
void disableBallCamera(void);

void updateBall(float interval);
void changeBall(int layout);

void activateBallShader(void);
void deactivateBallShader(void);

void drawMenuBall(void);
void drawGameBall(void);

int hasBallTexture(void);
int hasBallReflection(void);
int hasBallShader(void);

int useBallReflection(void);

#endif
