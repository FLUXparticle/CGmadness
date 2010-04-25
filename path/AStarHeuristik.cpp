#include "AStarHeuristik.hpp"

#include <cstdlib>

AStarHeuristik::AStarHeuristik()
{
	// empty
}

AStarHeuristik::~AStarHeuristik()
{
	// empty
}

Cost AStarHeuristik::estimate(NodeID cur, NodeID dest) const
{
	return abs(cur.coord.x - dest.coord.x) + abs(cur.coord.y - dest.coord.y);
}
