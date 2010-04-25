#ifndef WorldGraph_hpp
#define WorldGraph_hpp

#include "NodeGraph.hpp"

#include "World.hpp"

class WorldGraph: public NodeGraph
{
public:
	WorldGraph(const World& world);
	virtual ~WorldGraph();

	unsigned int size() const;
	NodeID followEdge(NodeID curNode, EdgeID eid) const;
	std::list<EdgeID> edges(NodeID curNode) const;
	Cost cost(EdgeID eid) const;

private:
	const World& mWorld;

	void addEdge(std::list<EdgeID>& list, EdgeID edge) const;

};

#endif
