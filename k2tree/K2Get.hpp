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

#ifndef K2Get_hpp
#define K2Get_hpp

#include "K2Command.hpp"

class K2Get : public K2Command {
public:
  K2Get();
  virtual ~K2Get();

	int decide(int close, int far);
	int hit(int index, const Range& range);
	int miss(int index);
	
	int index() const;
	
private:
	int mIndex;

};

inline int K2Get::index() const
{
	return mIndex;
}

#endif
