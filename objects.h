#ifndef _objects_h_
#define _objects_h_

#include "vector.h"

void initObjects(void);

void drawSquare(void);

void drawBallObject(int shader);

void initExplosion(Vector3 startPos, Vector3 startSpeed, Vector3 endPos, Vector3 endSpeed);
int updateExplosion(double interval, Vector3* speed, Vector3* pos);
void drawExplosion(int shader);

#endif
