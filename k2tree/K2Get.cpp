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

#include "K2Get.hpp"

K2Get::K2Get(const K2Tree& tree, const Vector2& q) :
	K2Iterator(tree, q)
{
	// empty
}

K2Get::~K2Get()
{
	// empty
}

int K2Get::decide(int close, int far)
{
	return close;
}

int K2Get::hit(int index, const Range& range)
{
	return -1;
}

int K2Get::miss(int index)
{
	return -1;
}
