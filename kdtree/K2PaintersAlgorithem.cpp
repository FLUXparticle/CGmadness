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

#include "K2PaintersAlgorithem.hpp"

K2PaintersAlgorithem::K2PaintersAlgorithem(const K2Tree& tree, const Vector3& viewer) :
	K2Iterator(tree, viewer)
{
	mStack.push(-1);
}

K2PaintersAlgorithem::~K2PaintersAlgorithem()
{
  // empty
}

int K2PaintersAlgorithem::decide(int close, int far)
{
	mStack.push(close);

	return far;
}

int K2PaintersAlgorithem::hit(int index)
{
	return pop();
}

int K2PaintersAlgorithem::miss(int index)
{
	return pop();
}

int K2PaintersAlgorithem::pop()
{
	int index = mStack.top();

	mStack.pop();

	return index;
}
