#ifndef Screen_hpp
#define Screen_hpp

#include "gui/MenuItem.hpp"

#include <list>

class Screen
{
public:
  Screen();
  virtual ~Screen();

  void prepare();
  
  virtual void show();
  
  void event(float x, float y, MouseEvent event);
  void update(float interval);
  
  void draw() const;

protected:
	std::list<MenuItem*> mItems;
	
};

#endif
