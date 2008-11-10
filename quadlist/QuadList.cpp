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

#include "QuadList.hpp"

QuadList::QuadList(KdIterator* iterator, const Vector3* vertices, const Vector3* normals) :
	mIterator(iterator),
	mVertices(vertices),
	mNormals(normals),
	mIndex(0),
	mEnd(0)
{
	// empty
}

bool QuadList::next()
{
	mIndex += 4;

	if (mIndex < mEnd)
	{
		return true;
	}
	else if (mIterator->next())
	{
		const KdCell::Range& range = **mIterator;

		mIndex = range.start;
		mEnd = range.end;

		return true;
	}

	return false;
}

Quad QuadList::operator*() const
{
	return Quad(mVertices + mIndex, mNormals + mIndex);
}
