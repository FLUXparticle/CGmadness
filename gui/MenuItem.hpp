#ifndef MenuItem_hpp
#define MenuItem_hpp

#include "math/Vector2.hpp"

typedef enum
{
	MI_CANVAS,
	MI_LABEL,
	MI_PROGRESS_BAR,
	MI_BUTTON,
	MI_CHECK,
	MI_SPIN_EDIT
} MenuItemType;

class MenuItem
{
public:
  MenuItem();
  virtual ~MenuItem();
  
  virtual void update(float interval);
  virtual void draw() const;

	MenuItemType type;

	Vector2 position;
	float width;
	float height;

	int hover;
	float emphasize;

private:

};

#endif
