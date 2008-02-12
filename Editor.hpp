#ifndef Editor_hpp
#define Editor_hpp

#include "Process.hpp"

class Editor : public Process
{
public:
  Editor();
  virtual ~Editor();

  void init();
  void start();
  
  void update(float interval);
  void draw(void);
  
private:

};

void initEditor(void);

void startEditor(void);
void stopEditor(void);

void resumeEditor(void);

void updateEditor(float interval);

void drawEditorField(void);

void drawEditor(void);

void saveLevel(void);

#endif
