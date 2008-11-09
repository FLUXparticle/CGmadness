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

#ifndef K2Tree_hpp
#define K2Tree_hpp

#include "KdTree.hpp"

#include "math/Vector3.hpp"
#include "K2Cell.hpp"

#include <vector>

class K2Tree: public KdTree
{
public:
	K2Tree(Vector3 origin, int sizeX, int sizeY);
	virtual ~K2Tree();

	void set(int x, int y, const QuadList& list);
	const QuadList& get(int x, int y) const;

	const K2Cell& cell(int index) const;
	K2Cell& cell(int index);

	int root() const;

private:
	int mRoot;
	Vector3 mOrigin;

	int mSizeX;
	int mSizeY;

	QuadList mEmpty;

	std::vector<K2Cell> mCells;

	int newNode(const K2Cell& cell);
	void split(int index, const K2Cell& left, const K2Cell& right);

	friend class K2Set;

};

inline const K2Cell& K2Tree::cell(int index) const
{
	return mCells[index];
}

inline K2Cell& K2Tree::cell(int index)
{
	return mCells[index];
}

inline int K2Tree::root() const
{
	return mRoot;
}

#endif
