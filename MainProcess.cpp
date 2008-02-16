#include "MainProcess.hpp"

#include "mainmenu.hpp"

MainProcess::MainProcess()
{
  // empty
}

MainProcess::~MainProcess()
{
  // empty
}

void MainProcess::init()
{
	initMainMenu();
	showMainMenu();
}

void MainProcess::start()
{
  // empty
}

void MainProcess::update(float interval)
{
	updateMainMenu(interval);
}

void MainProcess::draw(void)
{
	drawMainMenu();
}
