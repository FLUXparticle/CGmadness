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

#ifndef Block_hpp
#define Block_hpp

#include "math/Vector2.hpp"
#include "math/Vector3.hpp"

#include <vector>

struct Orientation
{
	Vector3 origin;
	Vector3 vx;
	Vector3 vy;
	Vector3 normal;

	Vector3 decomposition(const Vector3& v) const;
};

struct Square
{
	Vector3 vertices[4];
	Vector3 normal;

	Orientation texDecal;

	unsigned int idxAtlas;

	float area;
	Vector3 mid;

	void updateAttributes();

	Vector2 texCoords(int vertex) const;
};

struct SideFace
{
	std::vector<Square> squares;

	int bottom;
	int top;
};

struct Range
{
	int start;
	int end;
};

struct Block
{
	int x;
	int y;
	int z;
	int dzx;
	int dzy;

	bool dirty;

	Range subatlas;

	Square roof;
	SideFace sideFaces[4];
};

#endif
