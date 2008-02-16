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
  void stop();
  
  void update(float interval);

  void preDisplay();
  void draw();
  void drawHUD(float width, float height);

  void resumeGame();
  void resetGame();

  void resetGameTime();
  
private:
	bool gIsGameRunning;

	float gGameTime;

  void pauseGame();
  
	void stopWatch();
	void finishedGame();
	
};

void drawGameBallReflection(void);

#endif
