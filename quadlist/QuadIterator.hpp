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

#ifndef QuadIterator_hpp
#define QuadIterator_hpp

#include "Quad.hpp"

#include "math/Vector3.hpp"

class QuadIterator
{
public:
	QuadIterator(int index, const Vector3* vertices, const Vector3* normals);

	Quad operator*() const;
	bool operator==(const QuadIterator& other) const;
	bool operator!=(const QuadIterator& other) const;

	void operator++();

private:
	int mIndex;
	const Vector3* mVertices;
	const Vector3* mNormals;

};

#endif
