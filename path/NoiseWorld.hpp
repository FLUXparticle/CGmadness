#ifndef NoiseWorld_hpp
#define NoiseWorld_hpp

#include "World.hpp"

#include "math/Vector3.hpp"

class NoiseWorld: public World
{
public:
	NoiseWorld();
	virtual ~NoiseWorld();

	int getHeight(int x, int y) const;

private:
	struct Map
	{
		int data[WORLD_SIZE_X][WORLD_SIZE_Y];
	};
	static void genNoiseTexture(Map& map, Vector3 origin, Vector3 vx, Vector3 vy);

private:
	Map map;

};

#endif
