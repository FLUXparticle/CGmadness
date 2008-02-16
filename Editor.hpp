#ifndef Editor_hpp
#define Editor_hpp

#include "Process.hpp"

#include "Ball.hpp"

class Editor : public Process
{
public:
  static void init();
  
public:
  Editor();
  virtual ~Editor();

  void start();
  void stop();
  
  void update(float interval);
  void draw();
  
private:
	Ball mBall;
	
	void enableTestMode();
	void disableTestMode();

};

void resumeEditor(void);

void drawEditorField(void);

void saveLevel(void);

#endif
