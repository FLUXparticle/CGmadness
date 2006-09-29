#ifndef _ball_h_
#define _ball_h_

#include "graph.h"

#define BALL_RADIUS 0.2f

#define MAX_BALL_LAYOUTS 5

#define BALL_LAYOUT_DEFAULT 0
#define BALL_LAYOUT_BASKETBALL 1
#define BALL_LAYOUT_METAL 2
#define BALL_LAYOUT_GOLFBALL 3
#define BALL_LAYOUT_GOLFBALL_METAL 4

extern Object sgoBall;

void initBall(void);

void resetBall(void);

void updateBall(double interval);
void changeBall(int layout);

void activateBallShader(void);
void deactivateBallShader(void);

void drawMenuBall(void);
void drawGameBall(void);

int hasBallTexture(void);
int hasBallReflection(void);
int hasBallShader(void);

#endif
