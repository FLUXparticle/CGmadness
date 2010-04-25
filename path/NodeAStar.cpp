#include "NodeAStar.hpp"

#include "utils/BinaryHeap.hpp"
#include "macros.hpp"

#include <climits>
#include <queue>

NodeAStar::NodeAStar(const NodeGraph& graph, const Heuristic& heuristic) :
    mGraph(graph),
    mHeuristic(heuristic),
    mExpanded(0)
{
	// empty
}

NodeAStar::~NodeAStar()
{
	// empty
}

bool NodeAStar::execute(NodeID origin, NodeID destination, std::list<NodeID>& route)
{
    BinaryHeap queue(mGraph.size());
    NodeMap<Cost> dist;
    dist.put(origin, 0);
    queue.insert(dist[origin], origin);

    while (queue.size() > 0)
    {
        NodeID curNode = NodeID(queue.removeFirst());

        if (curNode == destination)
        {
            while (curNode != origin)
            {
                EdgeID eid = mPrevious[curNode];
                route.push_front(curNode);
                curNode = mGraph.followEdge(curNode, eid);
            }
            return true;
        }

        mExpanded++;

        std::list<EdgeID> edges = mGraph.edges(curNode);
        FOREACH(edges, iter)
        {
            EdgeID eid = *iter;
            NodeID neighborNode = mGraph.followEdge(curNode, eid);
            Cost cost = dist[curNode] + mGraph.cost(eid);
            Cost key = cost + mHeuristic.estimate(neighborNode, destination);

            if (!dist.exists(neighborNode) || cost < dist[neighborNode])
            {
                dist.put(neighborNode, cost);
                mPrevious.put(neighborNode, eid);
                queue.update(neighborNode, key);
            }
        }
    }

    return false;
}

#if (TREE)
std::list<EdgeID> NodeAStar::tree() const
{
    std::list<EdgeID> result;

    FOREACH(mPrevious, iter)
    {
        EdgeID eid = *iter;

        if (eid < mGraph.cntEdges())
        {
            result.push_back(eid);
        }
    }

    return result;
}
#endif
