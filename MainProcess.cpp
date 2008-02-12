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
}

void MainProcess::start()
{
	showMainMenu();
}

void MainProcess::update(float interval)
{
	updateMainMenu(interval);
}

void MainProcess::draw(void)
{
	drawMainMenu();
}
