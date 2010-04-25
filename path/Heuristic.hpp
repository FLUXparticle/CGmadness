#ifndef Heuristic_hpp
#define Heuristic_hpp

#include "NodeID.hpp"
#include "Cost.hpp"

class Heuristic
{
public:
	Heuristic();
	virtual ~Heuristic();

  virtual Cost estimate(NodeID cur, NodeID dest) const = 0;

private:

};

#endif
