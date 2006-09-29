#ifndef _vector_h_
#define _vector_h_

#include "types.h"

typedef struct {
	float x;
	float y;
} Vector2;

typedef struct {
	float x;
	float y;
	float z;
} Vector3;

float sqr(float x);

float len(Vector3 v);

void normalize(Vector3* v);

Vector3 norm(Vector3 v);

Vector3 scale(float s, Vector3 a);

Vector3 add(Vector3 a, Vector3 b);

Vector3 sub(Vector3 a, Vector3 b);

float dot(Vector3 a, Vector3 b);

Vector3 cross(Vector3 a, Vector3 b);

void initProjectMat(Matrix m, float fovy); 

#endif
