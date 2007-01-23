#ifndef _quaternion_h_
#define _quaternion_h_

#include "vector.h"

typedef struct {
	float s;
	Vector3 v;
} Quaternion;

float lenQuaternion(Quaternion q);

Quaternion normQuaternion(Quaternion q);

Quaternion scaleQuaternion(float s, Quaternion a);

Quaternion addQuaternion(Quaternion a, Quaternion b);

Quaternion subQuaternion(Quaternion a, Quaternion b);

float dotQuaternion(Quaternion a, Quaternion b);

Quaternion mulQuaternion(Quaternion a, Quaternion b);

Quaternion mkQuaternion(float alpha, Vector3 v);

Quaternion interpolate(float t, Quaternion a, Quaternion b);

void quaternionTransform(Quaternion a);

#endif
