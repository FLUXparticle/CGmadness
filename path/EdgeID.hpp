#ifndef EdgeID_hpp
#define EdgeID_hpp

#include "NodeID.hpp"

struct EdgeID
{
	EdgeID(NodeID a, NodeID b);

	NodeID a;
	NodeID b;
};

#endif
