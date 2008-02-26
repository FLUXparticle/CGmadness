#ifndef ScreenGameHelp_hpp
#define ScreenGameHelp_hpp

#include "Screen.hpp"

#include "gui/Label.hpp"
#include "gui/Button.hpp"

#include <vector>

class ScreenGameHelp : public Screen
{
public:
  ScreenGameHelp();
  virtual ~ScreenGameHelp();

private:
	Button bBack;
	std::vector<Label> lTextHelp;

};

#endif
