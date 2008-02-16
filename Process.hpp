#ifndef Process_hpp
#define Process_hpp

class Process
{
public:
  Process();
  virtual ~Process();
  
  virtual void start();
  virtual void stop();
  
  virtual void update(float interval) = 0;
  
  virtual void preDisplay();
  virtual void draw() = 0;
  virtual void drawHUD(float width, float height);

private:

};

#endif
