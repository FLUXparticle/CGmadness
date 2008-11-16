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

#include "KdList.hpp"

KdList::KdList(KdIterator& iterator) :
	mIterator(iterator),
	mIndex(0),
	mEnd(0)
{
	// empty
}

KdList::~KdList()
{
	// empty
}

bool KdList::next()
{
	mIndex++;
	if (mIndex < mEnd)
	{
		return true;
	}

	while (mIterator.next())
	{
		const KdCell::Range& range = *mIterator;

		mIndex = range.start;
		mEnd = range.end;

		if (mIndex < mEnd)
		{
			return true;
		}
	}

	return false;
}

int KdList::operator*() const
{
	return mIndex;
}
