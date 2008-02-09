#include "Dispenser.hpp"

Dispenser::Dispenser()
{
  // empty
}

Dispenser::~Dispenser()
{
  // empty
}

void Dispenser::update(float interval)
{
	mCurProcess->update(interval);
}

void Dispenser::draw(void)
{
	mCurProcess->draw();
}

void Dispenser::drawHUD(float width, float height)
{
	mCurProcess->drawHUD(width, height);
}

void Dispenser::setProcess(Process* process)
{
	if (mCurProcess)
	{
		mCurProcess->stop();
	}
	
	mCurProcess = process;
	
	if (mCurProcess)
	{
		mCurProcess->start();
	}
}
