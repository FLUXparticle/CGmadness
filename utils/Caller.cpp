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

#include "Caller.hpp"

#include "Function.hpp"

#include <stdlib.h>

struct FunctorRef
{
	Functor* mF;
	int mCounter;
	
	FunctorRef(Functor* f) :
		mF(f),
		mCounter(1)
	{
		// empty
	}
	~FunctorRef()
	{
		delete mF;
	}
};

Caller::Caller()
{
	assign(new FunctorRef(NULL));
}

Caller::Caller(const Caller& other)
{
	assign(other.mRef);
}

Caller::Caller(Functor* f)
{
	assign(new FunctorRef(f));
}

Caller::Caller(void (*f) ())
{
	assign(new FunctorRef(new Function(f)));
}

Caller::~Caller()
{
	release();
}

void Caller::operator() ()
{
	(*mRef->mF) ();
}

void Caller::operator= (const Caller& other)
{
	if (mRef != other.mRef)
	{
		release();
		assign(other.mRef);
	}
}

void Caller::release()
{
	mRef->mCounter--;
	if (mRef->mCounter == 0)
	{
		delete mRef;
	}
}

void Caller::assign(struct FunctorRef* ref)
{
	mRef = ref;
	mRef->mCounter++;
}
