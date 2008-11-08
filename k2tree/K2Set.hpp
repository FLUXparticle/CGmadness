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

#ifndef K2Set_hpp
#define K2Set_hpp

#include "K2Iterator.hpp"

class K2Set: public K2Iterator
{
public:
	K2Set(K2Tree& tree, const Vector3& q);
	virtual ~K2Set();

  Range& operator*();

private:
	K2Tree& mMutableTree;

	int decide(int close, int far);
	int hit(int index);
	int miss(int index);
};

#endif
