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

#include "ProgressBar.hpp"

#include "objects.hpp"

#include <GL/gl.h>

ProgressBar::ProgressBar()
{
  // empty
}

ProgressBar::ProgressBar(float z, float* progress)
{
	this->progress = progress;

	this->type = MI_PROGRESS_BAR;

	this->width = 8.0f;
	this->height = 0.9f;

	this->position = Vector2(0.0f, z);
}

ProgressBar::~ProgressBar()
{
  // empty
}

void ProgressBar::draw() const
{
	glPushMatrix();
	{
		glScalef(this->width / 2.0f, this->height / 2.0f, 1.0f);

		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

		drawSquare();

		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

		glScalef(*this->progress, 1.0f, 1.0f);

		drawSquare();
	}
	glPopMatrix();
}

