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

#include "K2Iterator.hpp"

K2Iterator::K2Iterator(const K2Tree& tree, const Vector3& q) :
	mIndex(-1),
	mTree(tree),
	mQ(q),
	mContinue(mTree.root())
{
  // empty
}

K2Iterator::~K2Iterator()
{
  // empty
}

bool K2Iterator::next()
{
	mIndex = mContinue;
	while (mIndex >= 0)
	{
		const K2Cell& cell = mTree.cell(mIndex);

		if (cell.sizeX == 1 && cell.sizeY == 1)
		{
			mContinue = hit(mIndex);
			return true;
		}
		else if (cell.left < 0)
		{
			mContinue = miss(mIndex);
		}
		else
		{
			const K2Cell& left = mTree.cell(cell.left);
			const K2Cell& right = mTree.cell(cell.right);

			Vector3 startL(left.startX, left.startY, 0.0f);
			Vector3 startR(right.startX, right.startY, 0.0f);

			Vector3 n = (startR - startL).norm();
			float d = startR * n;

			if (n * mQ < d)
			{
				mContinue = decide(cell.left, cell.right);
			}
			else
			{
				mContinue = decide(cell.right, cell.left);
			}
		}

		mIndex = mContinue;
	}

	return false;
}

const QuadList& K2Iterator::operator*() const
{
	return mTree.cell(mIndex).list;
}
