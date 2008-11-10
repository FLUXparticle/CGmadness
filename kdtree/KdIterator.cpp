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

#include "KdIterator.hpp"

KdIterator::KdIterator(const KdTree& tree, const Vector3& q) :
	mIndex(-1),
	mTree(tree),
	mQ(q),
	mContinue(mTree.root())
{
  // empty
}

KdIterator::~KdIterator()
{
  // empty
}

bool KdIterator::next()
{
	mIndex = mContinue;
	while (mIndex >= 0)
	{
		const KdCell& cell = mTree.cell(mIndex);

		Vector3 size = cell.max - cell.min;

		if (size.x * size.y * size.z <= 1)
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
			const KdCell& left = mTree.cell(cell.left);
			const KdCell& right = mTree.cell(cell.right);

			Vector3 n = (right.min - left.min).norm();
			float d = right.min * n;

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

const KdCell::Range& KdIterator::operator*() const
{
	return mTree.cell(mIndex).range;
}
