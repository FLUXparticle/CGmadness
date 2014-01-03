#ifndef LevelWorld_hpp
#define LevelWorld_hpp

#include "World.hpp"

#include "level/types.hpp"

class LevelWorld: public World
{
public:
	LevelWorld(const Level& level);
	virtual ~LevelWorld();

	int getHeight(int x, int y) const;

private:
	const Level& mLevel;

};

#endif
