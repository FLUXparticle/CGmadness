#ifndef NodeGraph_hpp
#define NodeGraph_hpp

#include "NodeID.hpp"
#include "EdgeID.hpp"
#include "Cost.hpp"

#include <list>

class NodeGraph
{
public:
	NodeGraph();
	virtual ~NodeGraph();

	virtual unsigned int size() const = 0;
	virtual NodeID followEdge(NodeID curNode, EdgeID eid) const = 0;
	virtual std::list<EdgeID> edges(NodeID curNode) const = 0;
	virtual Cost cost(EdgeID eid) const = 0;

private:

};

#endif
