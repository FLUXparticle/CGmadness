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

#include "KdSet.hpp"

#include <cmath>

KdSet::KdSet(KdTree& tree, const Vector3& q) :
	KdIterator(tree, q),
	mMutableTree(tree)
{
	// empty
}

KdSet::~KdSet()
{
	// empty
}

KdCell::Range& KdSet::operator*()
{
	return mMutableTree.cell(mIndex).range;
}


int KdSet::decide(int close, int far)
{
	return close;
}

int KdSet::hit(int index)
{
	return -1;
}

int KdSet::miss(int index)
{
	KdCell& cur = mMutableTree.cell(index);

	KdCell left = cur;
	KdCell right = cur;

	Vector3 size = cur.max - cur.min;

	if (size.x >= size.y && size.x >= size.z)
	{
		float mid = floor((cur.min.x + cur.max.x) / 2.0f);
		left.max.x = mid;
		right.min.x = mid;
	}
	else if (size.y >= size.x && size.y >= size.z)
	{
		float mid = floor((cur.min.y + cur.max.y) / 2.0f);
		left.max.y = mid;
		right.min.y = mid;
	}
	else
	{
		float mid = floor((cur.min.z + cur.max.z) / 2.0f);
		left.max.z = mid;
		right.min.z = mid;
	}

	mMutableTree.split(index, left, right);

	return index;
}
