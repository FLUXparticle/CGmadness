#ifndef Process_hpp
#define Process_hpp

class Process
{
public:
  Process();
  virtual ~Process();
  
  virtual void init();
  
  virtual void start();
  virtual void stop();
  
  virtual void update(float interval) = 0;
  virtual void draw(void) = 0;
  virtual void drawHUD(float width, float height);

private:

};

#endif
