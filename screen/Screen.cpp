#include "Screen.hpp"

#include "macros.hpp"

#include <GL/gl.h>

Screen::Screen()
{
  // empty
}

Screen::~Screen()
{
  // empty
}

void Screen::prepare()
{
  // TODO
}

void Screen::show()
{
  // empty
}

void Screen::event(float x, float y, MouseEvent event)
{
  // TODO
}

void Screen::update(float interval)
{
  FOREACH(std::list<MenuItem*>, mItems, iter)
  {
  	(*iter)->update(interval);
  }
}

static void drawMenuItem(const MenuItem* item)
{
	glPushMatrix();
	{
		glTranslatef(item->position.x, item->position.y, 0.0f);

		item->draw();
	}
	glPopMatrix();
}

void Screen::draw() const
{
  FOREACH(std::list<MenuItem*>, mItems, iter)
  {
  	drawMenuItem(*iter);
  }
}
