#ifndef Dispenser_hpp
#define Dispenser_hpp

#include "Process.hpp"

class Dispenser : public Process
{
public:
  Dispenser();
  virtual ~Dispenser();

	void setProcess(Process* process);
	
  void update(float interval);
  void draw(void);
  void drawHUD(float width, float height);

private:
	Process* mCurProcess;

};

#endif