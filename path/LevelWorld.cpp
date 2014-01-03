#include "LevelWorld.hpp"

#include "kdtree/KdGet.hpp"

#include "math/Vector3.hpp"

LevelWorld::LevelWorld(const Level& level):
	mLevel(level)
{
	// empty
}

LevelWorld::~LevelWorld()
{
	// empty
}

int LevelWorld::getHeight(int x, int y) const
{
		const KdCell::Range& range = mLevel.kdLevelTree->get(x, y);

		if (range.start < range.end)
		{
			const Block& b = sgLevel.blocks[range.start];

			return b.z / 5;
		}

		return 0;
}
