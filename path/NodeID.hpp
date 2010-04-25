#ifndef NodeID_hpp
#define NodeID_hpp

#include "level/types.hpp"

#include "World.hpp"

struct NodeID
{
	FieldCoord coord;

	NodeID(int x, int y)
	{
		coord.x = x;
		coord.y = y;
	}

	explicit NodeID(unsigned int value)
	{
		coord.x = value % WORLD_SIZE_X;
		coord.y = value / WORLD_SIZE_X;
	}

	operator unsigned int() const
	{
		return coord.y * WORLD_SIZE_X + coord.x;
	}
};

#endif
