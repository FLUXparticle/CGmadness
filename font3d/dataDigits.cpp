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

#include "dataDigits.hpp"

#include "Digits0.inc"
#include "Digits1.inc"
#include "Digits2.inc"
#include "Digits3.inc"
#include "Digits4.inc"
#include "Digits5.inc"
#include "Digits6.inc"
#include "Digits7.inc"
#include "Digits8.inc"
#include "Digits9.inc"

float widthDigits[10] = {
	10.000000,
	10.000000,
	10.000000,
	10.000000,
	10.000000,
	10.000000,
	10.000000,
	10.000000,
	10.000000,
	10.000000,
};

funcDraw drawDigits[10] = {
	drawDigits0,
	drawDigits1,
	drawDigits2,
	drawDigits3,
	drawDigits4,
	drawDigits5,
	drawDigits6,
	drawDigits7,
	drawDigits8,
	drawDigits9,
};
