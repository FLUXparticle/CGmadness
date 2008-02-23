#ifndef ProgressBar_hpp
#define ProgressBar_hpp

#include "MenuItem.hpp"

class ProgressBar : public MenuItem
{
public:
  ProgressBar();
  ProgressBar(float z, float* progress);
  virtual ~ProgressBar();

  void draw() const;
  
	float* progress;
	
private:

};

#endif
