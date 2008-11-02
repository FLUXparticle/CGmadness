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

#ifndef Callback_hpp
#define Callback_hpp

#include "Caller.hpp"

template<class T>
class Callback : public Functor
{
	T* mObject;
	void (T::*mF)();
	
public:
  Callback(T* object, void (T::*f)()) :
  	mObject(object),
  	mF(f)
  {
  	// empty
  }
  
  void operator() ()
  {
  	(mObject ->* mF) ();
  }
};

#define METHOD_CALLBACK(X, x) Caller(new Callback<X>(this, &X::x))

#endif
