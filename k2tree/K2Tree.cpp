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
	Vector3 q(x + 0.5f, y + 0.5f, 0.0f);
	K2Set iter(*this, q);

	if (iter.next())
	{
		Range &range = *iter;

		range.left = start;
		range.right = end;
	}
}

void K2Tree::get(int x, int y, int &start, int &end) const
{
	Vector3 q(x + 0.5f, y + 0.5f, 0.0f);
	K2Get iter(*this, q);

	if (iter.next())
	{
		const Range &range = *iter;

		start = range.left;
		end = range.right;
	}
}

int K2Tree::newNode(const Range &range)
{
	int index = mRanges.size();

	mRanges.push_back(range);

	return index;
}
