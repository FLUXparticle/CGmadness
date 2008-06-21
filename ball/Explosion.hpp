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

#ifndef Explosion_hpp
#define Explosion_hpp

#include "BallObject.hpp"

#define PARTS_TOGETHER 16

class Explosion : public BallObject
{
public:
	Explosion();
	virtual ~Explosion();

	void init(const Vector3& startPos, const Vector3& startSpeed,
			const Vector3& endPos, const Vector3& endSpeed);
	
	bool update(float interval, const Vector3& speed, Vector3& pos);

	void drawDefault() const;
	void drawShader() const;
	
private:
	struct Fragment
	{
		Vector3 vertices[PARTS_TOGETHER * 3];

		Vector3 pos;
		Vector3 offset;
		Vector3 speed;

		Vector3 rotation;
		Vector3 rotSpeed;
	};
	
	Vector3 gStartPos;
	Vector3 gStartSpeed;

	Vector3 gEndPos;
	Vector3 gEndSpeed;

	float gExplosionTime;
	float gMaxExplosionTime;

	Fragment gFragments[CNT_BALL_TRIANGLES / PARTS_TOGETHER];

	void draw(Vector3 ballTexCoords[CNT_BALL_VERTICES]) const;
	
};

#endif
