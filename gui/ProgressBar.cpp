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

