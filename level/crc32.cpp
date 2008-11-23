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

#include "crc32.hpp"

#define CRC32_POLYNOMIAL 0xEDB88320

unsigned int gRegister;

void resetCRC32()
{
	gRegister = 0xffffffff;
}

void nextByte(unsigned char byte)
{
	int i;
	for (i = 0; i < 8; i++)
	{
		int bit = (byte >> i) & 1;
		int regBit = gRegister & 1;

		gRegister >>= 1;

		if (regBit != bit)
		{
			gRegister ^= CRC32_POLYNOMIAL;
		}
	}
}

unsigned int getCRC32()
{
	return gRegister;
}

void setCRC32(unsigned int crc32)
{
	gRegister = crc32;
}
