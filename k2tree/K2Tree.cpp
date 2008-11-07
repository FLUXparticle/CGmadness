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

#include "K2Tree.hpp"

#include "K2Get.hpp"
#include "K2Set.hpp"
#include "K2PaintersAlgorithem.hpp"
#include "K2PaintersAlgorithemReverse.hpp"

#include "Range.hpp"

#include "field.hpp"

#include <cstdio>

K2Tree::K2Tree(Vector3 origin, int sizeX, int sizeY) :
	mOrigin(origin),
	mSizeX(sizeX),
	mSizeY(sizeY)
{
	mRoot = newNode(Range(0, 0, mSizeX, mSizeY));
}

K2Tree::~K2Tree()
{
  // empty
}

void K2Tree::set(int x, int y, int start, int end)
{
	Vector2 q(x + 0.5f, y + 0.5f);
	K2Set iter(*this, q);

	Range &range = *iter;

	range.left = start;
	range.right = end;
}

void K2Tree::get(int x, int y, int &start, int &end) const
{
	Vector2 q(x + 0.5f, y + 0.5f);
	K2Get iter(*this, q);

	const Range &range = *iter;

	start = range.left;
	end = range.right;
}

int K2Tree::newNode(const Range &range)
{
	int index = mRanges.size();

	mRanges.push_back(range);

	return index;
}

int painter(K2PaintersAlgorithem& iter, const Vector3& viewer, int indices[])
{
	int counter = 0;

	for (; iter.hasNext(); ++iter)
	{
		const Range &range = *iter;

		int start = range.left;
		int end = range.right;

		for (int q = start; q < end; q += 4)
		{
			/*
			 * the top square must always be drawn, because this function
			 * is not called if only the height of the camera changes.
			 * Fortunately it is always the first square in the array range.
			 * WARNING: be aware of this if you change the order of sqaures.
			 */
			if (q == start || dot(sgNormals[q], sub(viewer, sgVertices[q])) >= 0)
			{
				for (int i = 0; i < 4; i++)
				{
					indices[counter++] = q + i;
				}
			}
		}
	}

	return counter;
}

int K2Tree::paintersAlgorithem(const Vector3& viewer, int indices[]) const
{
	Vector3 diff = viewer - mOrigin;
	Vector2 q(diff.x, diff.y);
	K2PaintersAlgorithem iter(*this, q);

	return painter(iter, viewer, indices);
}

int K2Tree::paintersAlgorithemReverse(Vector3 viewer, int indices[]) const
{
	Vector3 diff = viewer - mOrigin;
	Vector2 q(diff.x, diff.y);
	K2PaintersAlgorithemReverse iter(*this, q);

	return painter(iter, viewer, indices);
}
