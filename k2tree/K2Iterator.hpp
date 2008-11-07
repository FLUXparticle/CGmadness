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

#ifndef K2Iterator_hpp
#define K2Iterator_hpp

#include "K2Tree.hpp"
#include "Range.hpp"

#include "math/Vector2.hpp"

class K2Iterator
{
public:
  K2Iterator(const K2Tree& tree, const Vector2& q);
  virtual ~K2Iterator();

  void operator++();

  bool hasNext() const;
  const Range& operator*();

protected:
	int mIndex;

private:
	const K2Tree& mTree;
	const Vector2& mQ;

	int mContinue;

	virtual int decide(int close, int far) = 0;
	virtual int hit(int index) = 0;
	virtual int miss(int index) = 0;

};

#endif
