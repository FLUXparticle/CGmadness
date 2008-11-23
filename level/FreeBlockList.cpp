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

#include "FreeBlockList.hpp"

FreeBlockIterator::FreeBlockIterator(class FreeBlockList* list, int index) :
	mList(list),
	mIndex(index)
{
	// empty
}

void FreeBlockIterator::operator++ ()
{
	mIndex = mList->mNodes[mIndex].next;
}

bool FreeBlockIterator::operator!= (const FreeBlockIterator& other) const
{
	return mList != other.mList || mIndex != other.mIndex;
}

Block& FreeBlockIterator::operator* ()
{
	return (*mList)[mIndex];
}

FreeBlockList::FreeBlockList() :
	mIdxFirstNode(0),
	mIdxLastNode(1),
	mIdxFirstFree(2),
	mIdxLastFree(3)
{
	clear();
}

FreeBlockList::~FreeBlockList()
{
	// empty
}

int FreeBlockList::insert(const Block& block)
{
	int index = mNodes[mIdxFirstFree].next;

	if (index == mIdxLastFree)
	{
		index = mNodes.size();
		mNodes.push_back(FreeBlockNode());
	}
	else
	{
		remove(index);
	}

	insertBefore(index, mIdxLastNode);
	mNodes[index].block = block;

	return index;
}

void FreeBlockList::free(int index)
{
	remove(index);
	insertBefore(index, mIdxLastFree);
}

void FreeBlockList::clear()
{
	mNodes.clear();

	FreeBlockNode node;

	node.prev = mIdxFirstNode;
	node.next = mIdxLastNode;

	mNodes.push_back(node);
	mNodes.push_back(node);

	node.prev = mIdxFirstFree;
	node.next = mIdxLastFree;

	mNodes.push_back(node);
	mNodes.push_back(node);
}

FreeBlockIterator FreeBlockList::begin()
{
	return FreeBlockIterator(this, mNodes[mIdxFirstNode].next);
}

FreeBlockIterator FreeBlockList::end()
{
	return FreeBlockIterator(this, mIdxLastNode);
}

const Block& FreeBlockList::operator[] (int index) const
{
	return mNodes[index].block;
}

Block& FreeBlockList::operator[] (int index)
{
	return mNodes[index].block;
}

void FreeBlockList::insertBefore(int newIndex, int index)
{
	FreeBlockNode& node = mNodes[newIndex];

	node.prev = mNodes[index].prev;
	node.next = index;

	mNodes[node.prev].next = newIndex;
	mNodes[node.next].prev = newIndex;
}

void FreeBlockList::remove(int index)
{
	FreeBlockNode& node = mNodes[index];

	mNodes[node.prev].next = node.next;
	mNodes[node.next].prev = node.prev;
}
