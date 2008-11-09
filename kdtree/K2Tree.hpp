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
#include "KdCell.hpp"

#include <vector>

class K2Tree: public KdTree
{
public:
	K2Tree(Vector3 origin, int sizeX, int sizeY);
	virtual ~K2Tree();

	void set(int x, int y, const QuadList& list);
	const QuadList& get(int x, int y) const;

	const KdCell& cell(int index) const;
	KdCell& cell(int index);

	int root() const;

private:
	int mRoot;
	Vector3 mOrigin;

	int mSizeX;
	int mSizeY;

	QuadList mEmpty;

	std::vector<KdCell> mCells;

	int newNode(const KdCell& cell);
	void split(int index, const KdCell& left, const KdCell& right);

	friend class K2Set;

};

inline const KdCell& K2Tree::cell(int index) const
{
	return mCells[index];
}

inline KdCell& K2Tree::cell(int index)
{
	return mCells[index];
}

inline int K2Tree::root() const
{
	return mRoot;
}

#endif
