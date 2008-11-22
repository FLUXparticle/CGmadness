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

#include "level/level.hpp"

#include "common.hpp"
#include "atlas.hpp"

#include "functions.hpp"

#include GL_H

#include <cmath>

const int sgEdgeX[4] = { 0, 1, 1, 0 };
const int sgEdgeY[4] = { 0, 0, 1, 1 };

Vector3 midpoint(const Vector3 quad[4])
{
	Vector3 mid = quad[0];

	for (int i = 1; i < 4; i++)
	{
		mid += quad[i];
	}

	return mid / 4.0f;
}

static float area3Points(const Vector3& a, const Vector3& b, const Vector3& c)
{
	return 0.5f * ((b - a) ^ (c - a)).len();
}

void Square::updateAttributes()
{
	for (int i = 0; i < 4; i++)
	{
		vertices[i] += sgLevel.origin;
	}

	mid = midpoint(vertices);

	float area1 = area3Points(vertices[0], vertices[1], vertices[3]);
	float area2 = area3Points(vertices[2], vertices[1], vertices[3]);

	area = area1 + area2;
}

static int getFieldEdgeHeight(int x, int y, int edge)
{
	if (x >= 0 && y >= 0 && x < sgLevel.size.x && y < sgLevel.size.y)
	{
		const KdCell::Range& range = sgLevel.kdLevelTree->get(x, y);

		if (range.start < range.end)
		{
			int dx = sgEdgeX[edge] * 2 - 1;
			int dy = sgEdgeY[edge] * 2 - 1;

			const Block& b = sgLevel.blocks[range.start];

			return 5 + b.z + dx * b.dzx + dy * b.dzy;
		}
	}

	/* value out of range */
	return 0;
}

Block& getBlock(int index)
{
	Block& b = sgLevel.blocks[index];

	checkBlock(b);

	return b;
}

void checkBlock(Block& b)
{
	int x = b.x;
	int y = b.y;

	if (b.dirty)
	{
		Square* square = &b.roof;

		for (int i = 0; i < 4; i++)
		{
			Vector3& v = square->vertices[i];
			v.x = x + sgEdgeX[i];
			v.y = y + sgEdgeY[i];
			v.z = (float) getFieldEdgeHeight(x, y, i) / HEIGHT_STEPS;
		}

		Vector3 ex = square->vertices[1] - square->vertices[0];
		Vector3 ey = square->vertices[3] - square->vertices[0];

		square->normal = norm(cross(ex, ey));

		square->updateAttributes();

		square->texDecal.origin = square->vertices[0];
		square->texDecal.vx = ex / floor(ex.len() + 0.5f);
		square->texDecal.vy = ey / floor(ey.len() + 0.5f);
		square->texDecal.normal = square->normal;

		for (int side = 0; side < 4; side++)
		{
			int next = (side + 1) % 4;
			int prev = (side + 3) % 4;

			int sideOpposite = (side + 3) % 4;
			int nextOpposite = (side + 2) % 4;

			int dx = sgEdgeX[side] - sgEdgeX[prev];
			int dy = sgEdgeY[side] - sgEdgeY[prev];

			int height1 = getFieldEdgeHeight(x, y, side);
			int height2 = getFieldEdgeHeight(x, y, next);

			int height3 = getFieldEdgeHeight(x + dx, y + dy, sideOpposite);
			int height4 = getFieldEdgeHeight(x + dx, y + dy, nextOpposite);

			SideFace* face = &b.sideFaces[side];

			Orientation sideDecal;

			const Square& roof = b.roof;
			sideDecal.origin = roof.vertices[side];
			sideDecal.vx = roof.vertices[next] - roof.vertices[side];
			sideDecal.vy = Vector3(0.0f, 0.0f, -1.0f);
			sideDecal.normal = Vector3(dx, dy, 0.0f);

			face->squares.clear();

			if (height1 > height3 || height2 > height4)
			{
				int x1 = x + sgEdgeX[side];
				int y1 = y + sgEdgeY[side];
				int x2 = x + sgEdgeX[next];
				int y2 = y + sgEdgeY[next];

				int startHeight = min(height3, height4);

				int minHeight;
				int maxHeight;
				int x3;
				int y3;

				int bottom;

				if (height1 > height2)
				{
					minHeight = height2;
					maxHeight = height1;
					x3 = x1;
					y3 = y1;
				}
				else
				{
					minHeight = height1;
					maxHeight = height2;
					x3 = x2;
					y3 = y2;
				}

				face->bottom = startHeight / HEIGHT_STEPS;
				face->top = ((maxHeight - 1) / HEIGHT_STEPS) + 1;

				for (bottom = startHeight; bottom < minHeight;)
				{
					int top =
						min(minHeight, bottom - (bottom % HEIGHT_STEPS) + HEIGHT_STEPS);

					face->squares.push_back(Square());
					square = &face->squares.back();

					square->normal = sideDecal.normal;

					square->vertices[0].x = x1;
					square->vertices[0].y = y1;
					square->vertices[0].z = (float) top / HEIGHT_STEPS;

					square->vertices[1].x = x1;
					square->vertices[1].y = y1;
					square->vertices[1].z = (float) bottom / HEIGHT_STEPS;

					square->vertices[2].x = x2;
					square->vertices[2].y = y2;
					square->vertices[2].z = (float) bottom / HEIGHT_STEPS;

					square->vertices[3].x = x2;
					square->vertices[3].y = y2;
					square->vertices[3].z = (float) top / HEIGHT_STEPS;

					square->texDecal = sideDecal;

					square->updateAttributes();

					bottom = top;
				}

				for (; bottom < maxHeight;)
				{
					int top =
						min(maxHeight, bottom - (bottom % HEIGHT_STEPS) + HEIGHT_STEPS);

					float t1 = (float) (bottom - minHeight) / (maxHeight - minHeight);
					float t2 = (float) (top - minHeight) / (maxHeight - minHeight);

					face->squares.push_back(Square());
					square = &face->squares.back();

					square->normal = sideDecal.normal;

					square->vertices[0].x = x1 * (1.0f - t2) + x3 * t2;
					square->vertices[0].y = y1 * (1.0f - t2) + y3 * t2;
					square->vertices[0].z = (float) top / HEIGHT_STEPS;

					square->vertices[1].x = x1 * (1.0f - t1) + x3 * t1;
					square->vertices[1].y = y1 * (1.0f - t1) + y3 * t1;
					square->vertices[1].z = (float) bottom / HEIGHT_STEPS;

					square->vertices[2].x = x2 * (1.0f - t1) + x3 * t1;
					square->vertices[2].y = y2 * (1.0f - t1) + y3 * t1;
					square->vertices[2].z = (float) bottom / HEIGHT_STEPS;

					square->vertices[3].x = x2 * (1.0f - t2) + x3 * t2;
					square->vertices[3].y = y2 * (1.0f - t2) + y3 * t2;
					square->vertices[3].z = (float) top / HEIGHT_STEPS;

					square->texDecal = sideDecal;

					square->updateAttributes();

					bottom = top;
				}
			}

		}

		b.dirty = false;
	}
}

void initLevel()
{
	sgLevel.blocks.clear();
	sgLevel.kdLevelTree = new KdTree(sgLevel.size.x, sgLevel.size.y);

	sgLevel.origin.x = -sgLevel.size.x / 2.0f;
	sgLevel.origin.y = 10.0f;
	sgLevel.origin.z = 0.0f;
}

void destroyLevel()
{
	delete sgLevel.kdLevelTree;

	sgLevel.blocks.clear();

	destroyCommon();
}

Block& getBlock(int x, int y)
{
	int index = sgLevel.kdLevelTree->get(x, y).start;
	return getBlock(index);
}

const Square& getRoofSquare(int x, int y)
{
	Block& b = getBlock(x, y);
	return b.roof;
}

const SideFace& getSideFace(int x, int y, int side)
{
	Block& b = getBlock(x, y);
	return b.sideFaces[side];
}
