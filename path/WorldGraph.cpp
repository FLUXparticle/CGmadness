#include "WorldGraph.hpp"

#include "NodeMap.hpp"

#include "functions.hpp"
#include "macros.hpp"

#include <queue>

#include <cstdio>
#include <cstdlib>
#include <cmath>

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

	std::list<NodeID> candidates;
	candidates.push_back(dest);

	while (!candidates.empty())
	{
		NodeID next = candidates.front();
		candidates.pop_front();

		bool free = true;
		std::list<NodeID> list = gbham(curNode.coord.x, curNode.coord.y, next.coord.x, next.coord.y);
		NodeID hit(next);
		FOREACH(list, iter)
		{
			NodeID nid = *iter;
			if (mWorld.getHeight(nid.coord.x, nid.coord.y) != 0)
			{
				free = false;
				hit = nid;
				break;
			}
		}

		if (free)
		{
			EdgeID eid(curNode, NodeID(next.coord.x, next.coord.y));
			result.push_back(eid);
			printf("(%d, %d) -> (%d, %d)\n", curNode.coord.x, curNode.coord.y, next.coord.x, next.coord.y);
			continue;
		}

		NodeMap<bool> closedList;
		std::list<NodeID> border;
		std::list<NodeID> island;

		island.push_back(hit);
		closedList.put(hit, true);
		while (!island.empty())
		{
			NodeID nid = island.front();
			island.pop_front();

			int x = nid.coord.x;
			int y = nid.coord.y;
			if (mWorld.getHeight(x, y) == 0)
			{
				border.push_back(nid);
				continue;
			}

			for (int dx = -1; dx <= 1; dx++)
			{
				for (int dy = -1; dy <= 1; dy++)
				{
					int nx = x + dx;
					int ny = y + dy;
					NodeID nnid(nx, ny);

					if (nx >= 0 && ny >= 0 && nx < WORLD_SIZE_X && ny < WORLD_SIZE_Y && !closedList.exists(nnid))
					{
						island.push_back(nnid);
						closedList.put(nnid, true);
					}
				}
			}
		}

		double minAngle = 0.0;
		double maxAngle = 0.0;
		const NodeID* minNid = NULL;
		const NodeID* maxNid = NULL;
		int ax = next.coord.x - curNode.coord.x;
		int ay = next.coord.y - curNode.coord.y;

		FOREACH(border, iter)
		{
			const NodeID& nid = *iter;

			int bx = nid.coord.x - curNode.coord.x;
			int by = nid.coord.y - curNode.coord.y;

			int cz = ax * by - ay * bx;

			double angle = cz / sqrt(sqr(bx) + sqr(by));

			if (angle <= minAngle)
			{
				minAngle = angle;
				minNid = &nid;
			}

			if (angle >= maxAngle)
			{
				maxAngle = angle;
				maxNid = &nid;
			}
		}

		candidates.push_back(*minNid);
		candidates.push_back(*maxNid);
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
