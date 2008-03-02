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

#include "MenuManager.hpp"

#include "screen/Screen.hpp"

#include "math/Vector3.hpp"

#include "camera.hpp"
#include "objects.hpp"
#include "texture.hpp"

#include <GL/gl.h>

MenuManager::MenuManager()
{
	gTexLogo = loadTexture("data/logo.tga", false);
}

MenuManager::~MenuManager()
{
  // empty
}

void MenuManager::pushScreen(Screen* screen)
{
	mScreenStack.push(screen);
	screen->show();
}

void MenuManager::popScreen()
{
	mScreenStack.pop();
	
	if (!mScreenStack.empty())
	{
		mScreenStack.top()->show();
	}
}

void MenuManager::update(float interval)
{
	Vector3 camera = Vector3(0.0f, -10.0f, 7.0f);
	Vector3 lookat = Vector3(0.0f, 0.0f, 5.0f);

	moveCamera(interval, camera, lookat);

	mScreenStack.top()->update(interval);
}

void MenuManager::event(const Vector3& position, const Vector3& direction, MouseEvent event)
{
	if (position.y < 0.0f && direction.y > 0.0f)
	{
		float t = -position.y / direction.y;
		float x = position.x + t * direction.x;
		float y = position.z + t * direction.z;

		mScreenStack.top()->event(x, y, event);
	}
}

void MenuManager::drawLogo()
{
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, gTexLogo);
	{
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		{
			glPushMatrix();
			{
				glTranslatef(0.0f, 8.0f, 0.0f);
				glScalef(4.0f, 1.0f, 1.0f);

				drawSquare();
			}
			glPopMatrix();
		}
		glDisable(GL_BLEND);
	}
	glDisable(GL_TEXTURE_2D);
}

void MenuManager::draw()
{
	mScreenStack.top()->drawBackground();
	
	float pos[4] = { 0.0f, -1.0f, 0.5f, 0.0f };

	glEnable(GL_LIGHT0);
	glLightfv(GL_LIGHT0, GL_POSITION, pos);

	glEnable(GL_LIGHTING);
	glEnable(GL_COLOR_MATERIAL);

	glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);
	{

		glColor3f(1.0f, 1.0f, 1.0f);

		glPushMatrix();
		{
			glRotatef(90.0f, 1.0f, 0.0f, 0.0f);

			drawLogo();
			mScreenStack.top()->draw();
		}
		glPopMatrix();
	}
	glDisable(GL_COLOR_MATERIAL);
	glDisable(GL_LIGHTING);
	glDisable(GL_LIGHT0);

}
