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

#ifndef FreeBlockList_hpp
#define FreeBlockList_hpp

#include "Block.hpp"

#include <vector>

class FreeBlockIterator
{
public:
	FreeBlockIterator(class FreeBlockList* list, int index);

	void operator++ ();
	bool operator!= (const FreeBlockIterator& other) const;
	Block& operator* ();

private:
	class FreeBlockList* mList;
	int mIndex;

};

struct FreeBlockNode
{
	Block block;
	int prev;
	int next;
};

class FreeBlockList
{
public:
	FreeBlockList();
	virtual ~FreeBlockList();

	int insert(const Block& block);
	void free(int index);
	void clear();

	FreeBlockIterator begin();
	FreeBlockIterator end();

	const Block& operator[] (int index) const;
	Block& operator[] (int index);

private:
	friend class FreeBlockIterator;

	std::vector<FreeBlockNode> mNodes;
	int mIdxFirstNode;
	int mIdxLastNode;
	int mIdxFirstFree;
	int mIdxLastFree;

	void insertBefore(int newIndex, int index);
	void remove(int index);

};

#endif
