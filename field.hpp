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

#ifndef field_hpp
#define field_hpp

#include "ball/PlayersBall.hpp"

#include "math/Vector3.hpp"

struct Quad
{
	const Vector3* mVertices;
	const Vector3* mNormals;

	Quad(const Vector3* vertices, const Vector3* normals);
};

class QuadList
{
public:
	class QuadIterator
	{
	public:
		QuadIterator(int index);

		Quad operator*() const;
		bool operator!=(const QuadIterator& other) const;

		void operator++();

	private:
		int mIndex;

	};

public:
	QuadList(int start, int end);

	QuadIterator begin() const;
	QuadIterator end() const;

private:
	int mStart;
	int mEnd;

};

void initGameField();
void destroyGameField();

void updateGameField(const PlayersBall& ball);
void drawGameField(bool ballReflection);

QuadList getQuadList(int x, int y);

#endif
