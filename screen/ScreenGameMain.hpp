#ifndef ScreenGameMain_hpp
#define ScreenGameMain_hpp

#include "Screen.hpp"

#include "gui/Button.hpp"
#include "gui/Check.hpp"
#include "gui/SpinEdit.hpp"

#include <vector>

class ScreenGameMain : public Screen
{
public:
  ScreenGameMain();
  virtual ~ScreenGameMain();
  
  void show();

protected:
	std::vector<int> gBallLayouts;

	Button bQuit;
	Button bHelp;

	Check gcBallShadow;
	Check gcReflection;

	SpinEdit gseBall;
	
};

#endif
