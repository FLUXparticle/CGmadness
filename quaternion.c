#include "quaternion.h"

#include <GL/gl.h>

#include <math.h>

/*
 * Dieses Modul f�hrt Vektor rechenoberationen aus. Die einzelnen Funktionen sind selbst erkl�rend.
 */

float lenQuaternion(Quaternion q) {
	return sqrt(sqr(q.s) + sqr(len(q.v)));
}

Quaternion normQuaternion(Quaternion q) {
	float f = 1.0f / lenQuaternion(q);

	q.s *= f;
	q.v = scale(f, q.v);

	return q;
}

Quaternion scaleQuaternion(float s, Quaternion q) {
	q.s *= s;
	q.v = scale(s, q.v);

	return q;
}

Quaternion addQuaternion(Quaternion a, Quaternion b) {
	Quaternion c;

	c.s = a.s + b.s;
	c.v = add(a.v, b.v);

	return c;
}

Quaternion subQuaternion(Quaternion a, Quaternion b) {
	Quaternion c;

	c.s = a.s - b.s;
	c.v = sub(a.v, b.v);

	return c;
}

float dotQuaternion(Quaternion a, Quaternion b) {
	return a.s * b.s + dot(a.v, b.v);
}

Quaternion mulQuaternion(Quaternion a, Quaternion b) {
	Quaternion c;

	c.s = a.s * b.s - dot(a.v, b.v);

	c.v = add(cross(a.v, b.v), add(scale(a.s, b.v), scale(b.s, a.v)));

	return c;
}

Quaternion mkQuaternion(float alpha, Vector3 v)
{
	float a = alpha * PI / 360.0f;
	float s = sin(a);
	float c = cos(a);
	Quaternion q;

	q.s = c;
	q.v = scale(s, norm(v));

	return q;
}

Quaternion interpolate(float t, Quaternion a, Quaternion b)
{
	float w = acos(dotQuaternion(normQuaternion(a), normQuaternion(b)));
	float s = sin(w);

	return addQuaternion(
			scaleQuaternion(sin((1.0f - t) * w) / s, a),
			scaleQuaternion(sin(        t  * w) / s, b)
		);
}

void quaternionTransform(Quaternion q)
{
	float m[16];
	float s = q.s;
	float x = q.v.x;
	float y = q.v.y;
	float z = q.v.z;

	m[ 0] = 1.0f - 2.0f * (  y * y + z * z);
	m[ 1] =        2.0f * (  s * z + x * y);
	m[ 2] =        2.0f * (- s * y + x * z);
	m[ 3] =        0.0f;

	m[ 4] =        2.0f * (- s * z + x * y);
	m[ 5] = 1.0f - 2.0f * (  x * x + z * z);
	m[ 6] =        2.0f * (  s * x + y * z);
	m[ 7] =        0.0f;

	m[ 8] =        2.0f * (  s * y + x * z);
	m[ 9] =        2.0f * (- s * x + y * z);
	m[10] = 1.0f - 2.0f * (  x * x + y * y);
	m[11] =        0.0f;

	m[12] =        0.0f;
	m[13] =        0.0f;
	m[14] =        0.0f;
	m[15] =        1.0f;
	
	glMultMatrixf(m);
}
