#ifndef RaceTheClock_hpp
#define RaceTheClock_hpp

#include "Game.hpp"

class RaceTheClock : public Game
{
public:
  RaceTheClock();
  virtual ~RaceTheClock();
  
  void resetGame();
  
  void update(float interval);
  void drawHUD(float width, float height);

  void resetGameTime();
  
private:
	float gGameTime;

	void stopWatch();
	void finishedGame();
	
};

#endif
