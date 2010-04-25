#include "WorldGraph.hpp"

#include "functions.hpp"
#include "macros.hpp"

#include <cstdlib>

WorldGraph::WorldGraph(const World& world, NodeID destination):
	mWorld(world),
	mDestination(destination)
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
	return edges(curNode, mDestination);
}

Cost WorldGraph::cost(EdgeID eid) const
{
	return abs(eid.a.coord.x - eid.b.coord.x) + abs(eid.a.coord.y - eid.b.coord.y);
}

static std::list<NodeID> gbham(int xstart, int ystart, int xend, int yend)
{
	std::list<NodeID> result;
	int x, y, t, dx, dy, incx, incy, pdx, pdy, ddx, ddy, es, el, err;

	/* Entfernung in beiden Dimensionen berechnen */
	dx = xend - xstart;
	dy = yend - ystart;

	/* Vorzeichen des Inkrements bestimmen */
	incx = sign(dx);
	incy = sign(dy);
	if(dx<0) dx = -dx;
	if(dy<0) dy = -dy;

	/* feststellen, welche Entfernung größer ist */
	if (dx>dy)
	{
		/* x ist schnelle Richtung */
		pdx=incx; pdy=0;    /* pd. ist Parallelschritt */
		ddx=incx; ddy=incy; /* dd. ist Diagonalschritt */
		es =dy;   el =dx;   /* Fehlerschritte schnell, langsam */
	} else
	{
		/* y ist schnelle Richtung */
		pdx=0;    pdy=incy; /* pd. ist Parallelschritt */
		ddx=incx; ddy=incy; /* dd. ist Diagonalschritt */
		es =dx;   el =dy;   /* Fehlerschritte schnell, langsam */
	}

	/* Initialisierungen vor Schleifenbeginn */
	x = xstart;
	y = ystart;
	err = el/2;
	result.push_back(NodeID(x, y));

	/* Pixel berechnen */
	for(t=0; t<el; ++t) /* t zaehlt die Pixel, el ist auch Anzahl */
	{
		/* Aktualisierung Fehlerterm */
		err -= es;
		if(err<0)
		{
			/* Fehlerterm wieder positiv (>=0) machen */
			err += el;
			/* Schritt in langsame Richtung, Diagonalschritt */
			x += ddx;
			y += ddy;
		} else
		{
			/* Schritt in schnelle Richtung, Parallelschritt */
			x += pdx;
			y += pdy;
		}
		result.push_back(NodeID(x, y));
	}

	return result;
}

std::list<EdgeID> WorldGraph::edges(NodeID curNode, NodeID dest) const
{
	std::list<EdgeID> result;

	bool free = true;
	std::list<NodeID> list = gbham(curNode.coord.x, curNode.coord.y, dest.coord.x, dest.coord.y);
	FOREACH(list, iter)
	{
		NodeID nid = *iter;
		if (mWorld.getHeight(nid.coord.x, nid.coord.y) != 0)
		{
			free = false;
			break;
		}
	}

	if (free)
	{
		EdgeID eid(curNode, NodeID(dest.coord.x, dest.coord.y));
		result.push_back(eid);
		return result;
	}

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

void WorldGraph::addEdge(std::list<EdgeID>& list, EdgeID edge) const
{
	int x = edge.b.coord.x;
	int y = edge.b.coord.y;

	if (mWorld.getHeight(x, y) == 0)
	{
		list.push_back(edge);
	}
}
