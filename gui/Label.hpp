#ifndef Label_hpp
#define Label_hpp

#include "MenuItem.hpp"

class Label : public MenuItem
{
public:
  Label();
  Label(float x, float z, float size, bool alignRight, char* text);
  virtual ~Label();

  void draw() const;
  
	char* text;
	float size;

private:

};

#endif
