#ifndef ScreenGameEnd_hpp
#define ScreenGameEnd_hpp

#include "Screen.hpp"

#include "highscore.hpp"

#include "gui/Button.hpp"

class ScreenGameEnd : public Screen
{
public:
  ScreenGameEnd();
  virtual ~ScreenGameEnd();

private:
	HighScore hsHighScore;
	Button bAgain;
	Button bQuit2;

};

#endif
