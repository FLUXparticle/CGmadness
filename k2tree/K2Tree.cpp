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

#include <math.h>

K2Tree::K2Tree(Vector3 origin, int x, int y) :
	mOrigin(origin),
	mX(x),
	mY(y)
{
	newNode(Range(0, 0, mX, mY));
}

K2Tree::~K2Tree()
{
  // empty
}

void K2Tree::set(int x, int y, int start, int end)
{
	K2Set cmd(*this);
	
	find(x, y, cmd);
	
	Range &range = mRanges[cmd.index()];
	
	range.left = start;
	range.right = end;
}

void K2Tree::get(int x, int y, int &start, int &end) const
{
	K2Get cmd;
	
	find(x, y, cmd);
	
	const Range &range = mRanges[cmd.index()];
	
	start = range.left;
	end = range.right;
}

int K2Tree::newNode(const Range &range)
{
	int index = mRanges.size();
		
	mRanges.push_back(range);
	
	return index;
}

int K2Tree::paintersAlgorithem(Vector3 viewer, int indices[]) const
{
	K2PaintersAlgorithem painter(viewer, indices);
	
	Vector3 diff = sub(viewer, mOrigin);
	
	int vx = (int) floor(diff.x);
	int vy = (int) floor(diff.y);
	
	find(vx, vy, painter);
	
	return painter.count();
}

int K2Tree::paintersAlgorithemReverse(Vector3 viewer, int indices[]) const
{
	K2PaintersAlgorithemReverse painter(viewer, indices);
	
	Vector3 diff = sub(viewer, mOrigin);
	
	int vx = (int) floor(diff.x);
	int vy = (int) floor(diff.y);
	
	find(vx, vy, painter);
	
	return painter.count();
}

void K2Tree::find(int x, int y, K2Command& cmd) const
{
	int index = 0;
	
	do
	{
		const Range& cur = mRanges[index];
		
		if (cur.sizeX == 0 || cur.sizeY == 0)
		{
			return;
		}
		else if (cur.sizeX == 1 && cur.sizeY == 1)
		{
			index = cmd.hit(index, cur);
		}
		else if (cur.right == 0)
		{
			index = cmd.miss(index);
		}
		else
		{
			const Range& right = mRanges[cur.right];
			
			if ((cur.sizeX > cur.sizeY && x < right.startX) || (cur.sizeX <= cur.sizeY && y < right.startY))
			{
				index = cmd.decide(cur.left, cur.right);
			}
			else
			{
				index = cmd.decide(cur.right, cur.left);
			}
		}
	} while (index >= 0);
}
