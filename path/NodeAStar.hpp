#ifndef NodeAStar_hpp
#define NodeAStar_hpp

#include "NodeID.hpp"
#include "EdgeID.hpp"

#include "Heuristic.hpp"
#include "NodeGraph.hpp"
#include "NodeMap.hpp"

#include "utils/StaticArray.hpp"

#include <list>

#define TREE 0

class NodeAStar
{
public:
	NodeAStar(const NodeGraph& graph, const Heuristic& heuristic);
	virtual ~NodeAStar();

    bool execute(NodeID origin, NodeID destination, std::list<NodeID>& route);

    unsigned int expanded() const;
#if (TREE)
    std::list<EdgeID> tree() const;
#endif

private:
    const NodeGraph& mGraph;
    const Heuristic& mHeuristic;

    NodeMap<EdgeID> mPrevious;

    unsigned int mExpanded;

};

inline unsigned int NodeAStar::expanded() const
{
    return mExpanded;
}

#endif
