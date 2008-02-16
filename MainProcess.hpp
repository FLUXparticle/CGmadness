#ifndef MainProcess_hpp
#define MainProcess_hpp

#include "Process.hpp"

class MainProcess : public Process
{
public:
  static void init();

public:
  MainProcess();
  virtual ~MainProcess();

  void start();
  
  void update(float interval);
  void draw(void);

private:

};

#endif
