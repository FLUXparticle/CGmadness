#include "WorldGraph.hpp"

#include <cstdlib>

WorldGraph::WorldGraph(const World& world):
	mWorld(world)
{
	// empty
}

WorldGraph::~WorldGraph()
{
	// empty
}

unsigned int WorldGraph::size() const
{
	return WORLD_SIZE;
}

NodeID WorldGraph::followEdge(NodeID curNode, EdgeID eid) const
{
	return (curNode == eid.a) ? eid.b : eid.a;
}

std::list<EdgeID> WorldGraph::edges(NodeID curNode) const
{
	std::list<EdgeID> result;

	int x = curNode.coord.x;
	int y = curNode.coord.y;

	if (x > 0)
	{
		EdgeID eid(curNode, NodeID(x - 1, y));
		addEdge(result, eid);
	}

	if (y > 0)
	{
		EdgeID eid(curNode, NodeID(x, y - 1));
		addEdge(result, eid);
	}

	if (x < WORLD_SIZE_X - 1)
	{
		EdgeID eid(curNode, NodeID(x + 1, y));
		addEdge(result, eid);
	}

	if (y < WORLD_SIZE_Y - 1)
	{
		EdgeID eid(curNode, NodeID(x, y + 1));
		addEdge(result, eid);
	}

	return result;
}

Cost WorldGraph::cost(EdgeID eid) const
{
	return abs(eid.a.coord.x - eid.b.coord.x) + abs(eid.a.coord.y - eid.b.coord.y);
}

void WorldGraph::addEdge(std::list<EdgeID>& list, EdgeID edge) const
{
	int x = edge.b.coord.x;
	int y = edge.b.coord.y;

	if (mWorld.getHeight(x, y) == 0)
	{
		list.push_back(edge);
	}
}
