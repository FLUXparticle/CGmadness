#ifndef DijkstraHeuristic_hpp
#define DijkstraHeuristic_hpp

#include "Heuristic.hpp"

class DijkstraHeuristic: public Heuristic
{
public:
	DijkstraHeuristic();
	virtual ~DijkstraHeuristic();

  Cost estimate(NodeID cur, NodeID dest) const;

private:

};

#endif
