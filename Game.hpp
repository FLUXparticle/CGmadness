#ifndef Game_hpp
#define Game_hpp

#include "Process.hpp"

class Game : public Process
{
public:
  Game();
  virtual ~Game();

  void init();
  void start();
  
  void update(float interval);
  void draw(void);
  void drawHUD(float width, float height);

private:

};

void initGame(void);
void resumeGame(void);
void resetGame(void);

void startGame(void);
void stopGame(void);

void resetGameTime(void);

void updateGame(float interval);

void drawGame(void);
void drawGameBallReflection(void);
void drawGameHUD(float widthWindow, float heightWindow);

#endif
