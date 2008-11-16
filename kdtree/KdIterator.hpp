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

#ifndef KdIterator_hpp
#define KdIterator_hpp

#include "KdTree.hpp"
#include "KdCell.hpp"

#include "math/Vector3.hpp"

class KdIterator
{
public:
  KdIterator(const KdTree& tree, const Vector3& q);
  virtual ~KdIterator();

  bool next();
  const KdCell::Range& operator*() const;

  int index() const;

protected:
	int mIndex;

	const KdTree& mTree;
	Vector3 mQ;

private:
	int mContinue;

	virtual int decide(int close, int far) = 0;
	virtual int hit(int index) = 0;
	virtual int miss(int index) = 0;

};

inline int KdIterator::index() const
{
	return mIndex;
}

#endif
