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

#include "types.hpp"

#include "atlas.hpp"

Level sgLevel;

Vector3 Orientation::decomposition(const Vector3& v) const
{
	Vector3 a = v - origin;

	float d = (vx ^ vy) * normal;
	float dx = (a ^ vy) * normal;
	float dy = (vx ^ a) * normal;
	float dz = (vx ^ vy) * a;

	return Vector3(dx / d, dy / d, dz / d);
}

Vector2 Square::texCoords(int vertex) const
{
	Vector3 decomp = texDecal.decomposition(vertices[vertex]);

	return Vector2(decomp.x, decomp.y);
}

Vector2 Square::lightmapCoords(int vertex) const
{
	static float a = (float) (LIGHT_MAP_SIZE - 1) / LIGHT_MAP_SIZE;
	static float b = 1.0f / (2 * LIGHT_MAP_SIZE);

	Vector3 decomp = atlas->orientation.decomposition(vertices[vertex]);

	Vector2 result;

	result.x = a * decomp.x + b;
	result.y = a * decomp.y + b;

	transformCoords(atlas->idxSubLightMap, result);

	return result;
}
