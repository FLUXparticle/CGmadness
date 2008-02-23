#include "Label.hpp"

#include "text.hpp"

#include <GL/gl.h>

Label::Label()
{
  // empty
}

Label::Label(float x, float z, float size, bool alignRight, char* text)
{
	this->text = text;
	this->size = size;

	this->type = MI_LABEL;

	this->width = widthFont3DText(this->text) * scaleText * size;
	this->height = 0.9f * size;

	if (alignRight)
	{
		this->position = Vector2(x - this->width, z);
	}
	else
	{
		this->position = Vector2(x, z);
	}
}

Label::~Label()
{
  // empty
}

void Label::draw() const
{
	float scale = scaleText * this->size;

	glPushMatrix();
	{
		glScalef(scale, scale, scale);
	
		drawFont3DText(this->text);
	}
	glPopMatrix();
}

