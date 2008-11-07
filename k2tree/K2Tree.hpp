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

#include "math/Vector3.hpp"
#include "Range.hpp"

#include <vector>

class K2Tree
{
public:
	K2Tree(Vector3 origin, int sizeX, int sizeY);
	virtual ~K2Tree();

	void set(int x, int y, int start, int end);
	void get(int x, int y, int &start, int &end) const;

	int paintersAlgorithem(const Vector3& viewer, int indices[]) const;
	int paintersAlgorithemReverse(Vector3 viewer, int indices[]) const;

	const Range& range(int index) const;
	Range& range(int index);

	int root() const;

private:
	int mRoot;
	Vector3 mOrigin;

	int mSizeX;
	int mSizeY;

	std::vector<Range> mRanges;

	int newNode(const Range &range);

	friend class K2Set;

};

inline const Range& K2Tree::range(int index) const
{
	return mRanges[index];
}

inline Range& K2Tree::range(int index)
{
	return mRanges[index];
}

inline int K2Tree::root() const
{
	return mRoot;
}

#endif
