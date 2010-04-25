#ifndef AStarHeuristik_hpp
#define AStarHeuristik_hpp

#include "Heuristic.hpp"

class AStarHeuristik: public Heuristic
{
public:
	AStarHeuristik();
	virtual ~AStarHeuristik();

  Cost estimate(NodeID cur, NodeID dest) const;

private:

};

#endif
