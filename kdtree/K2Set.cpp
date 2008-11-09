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

#include "K2Set.hpp"

K2Set::K2Set(K2Tree& tree, const Vector3& q) :
	K2Iterator(tree, q),
	mMutableTree(tree)
{
	// empty
}

K2Set::~K2Set()
{
	// empty
}

QuadList& K2Set::operator*()
{
	return mMutableTree.cell(mIndex).list;
}


int K2Set::decide(int close, int far)
{
	return close;
}

int K2Set::hit(int index)
{
	return -1;
}

int K2Set::miss(int index)
{
	K2Cell& cur = mMutableTree.cell(index);

	K2Cell left = cur;
	K2Cell right = cur;

	if (cur.sizeX > cur.sizeY)
	{
		left.sizeX = cur.sizeX / 2;
		right.sizeX = cur.sizeX - left.sizeX;
		right.startX = left.startX + left.sizeX;
	}
	else
	{
		left.sizeY = cur.sizeY / 2;
		right.sizeY = cur.sizeY - left.sizeY;
		right.startY = left.startY + left.sizeY;
	}

	mMutableTree.split(index, left, right);

	return index;
}
