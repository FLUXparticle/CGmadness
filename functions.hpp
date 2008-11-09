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

#ifndef functions_hpp
#define functions_hpp

template <class T>
T sqr(T x)
{
	return x * x;
}

template <class T>
T min(T a, T b)
{
	return (a < b) ? a : b;
}

template <class T>
T max(T a, T b)
{
	return (a > b) ? a : b;
}

template <class T>
T clamp(T x, T lo, T up)
{
	return min(up, max(lo, x));
}

template <class T>
bool between(T x, T min, T max)
{
	return x >= min && x <= max;
}

#endif
