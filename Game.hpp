#ifndef Game_hpp
#define Game_hpp

#include "Process.hpp"

#include "PlayersBall.hpp"

class Game : public Process
{
public:
	static void init();
  
public:
  Game();
  virtual ~Game();

  void start();
  void stop();
  
  void update(float interval);

  void preDisplay();
  void draw();

  void resumeGame();
  virtual void resetGame();

	void drawWaterReflection() const;
	void drawGameBallReflection() const;
	
protected:
  static PlayersBall& sgoBall;
  
protected:
	bool gIsGameRunning;

  void pauseGame();
  
};

#endif
