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

#include "Matrix.hpp"

#include <math.h>

/*
 * projection matrix without farplane
 */
void initProjectMat(Matrix m, float fov)
{
	int x;
	int y;
	float f = 1.0f / tan(fov / 2.0f * M_PI / 180.0f);
	static float near = 0.01f;

	for (x = 0; x < 4; x++)
	{
		for (y = 0; y < 4; y++)
		{
			m[x][y] = 0.0f;
		}
	}

	m[0][0] = f;

	m[1][1] = f;

	m[2][2] = -1;
	m[2][3] = -1;

	m[3][2] = -2 * near;
}
