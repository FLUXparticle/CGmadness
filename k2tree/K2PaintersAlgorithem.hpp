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

#ifndef K2PaintersAlgorithem_hpp
#define K2PaintersAlgorithem_hpp

#include "K2Iterator.hpp"

#include "math/Vector3.hpp"

#include <stack>

class K2PaintersAlgorithem: public K2Iterator
{
public:
	K2PaintersAlgorithem(const K2Tree& tree, const Vector2& viewer);
	virtual ~K2PaintersAlgorithem();

	int decide(int close, int far);
	int hit(int index, const Range& range);
	int miss(int index);

protected:
	std::stack<int> mStack;

private:
	int pop();

};

#endif
