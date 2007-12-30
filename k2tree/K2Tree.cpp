#include "K2Tree.hpp"

#include "K2Get.hpp"
#include "K2Set.hpp"

#include "Range.hpp"

K2Tree::K2Tree(Vector3 origin, int x, int y) :
	mOrigin(origin),
	mX(x),
	mY(y)
{
	newNode(Range(0, 0, mX, mY));
}

K2Tree::~K2Tree()
{
  // empty
}

void K2Tree::set(int x, int y, int start, int end)
{
	K2Set cmd(*this);
	
	find(x, y, cmd);
	
	Range &range = mRanges[cmd.index()];
	
	range.left = start;
	range.right = end;
}

void K2Tree::get(int x, int y, int &start, int &end) const
{
	K2Get cmd;
	
	find(x, y, cmd);
	
	const Range &range = mRanges[cmd.index()];
	
	start = range.left;
	end = range.right;
}

int K2Tree::newNode(const struct Range &range)
{
	int index = mRanges.size();
		
	mRanges.push_back(range);
	
	return index;
}

void K2Tree::find(int x, int y, K2Command& cmd) const
{
	int index = 0;
	
	do
	{
		const Range& cur = mRanges[index];
		
		if (cur.sizeX == 0 || cur.sizeY == 0)
		{
			return;
		}
		else if (cur.sizeX == 1 && cur.sizeY == 1)
		{
			index = cmd.hit(index);
		}
		else
		{
			if (cur.right == 0)
			{
				index = cmd.miss(index);
			}
			else
			{
				const Range& right = mRanges[cur.right];
				
				if (x < right.startX || y < right.startY)
				{
					index = cmd.decide(cur.left, cur.right);
				}
				else
				{
					index = cmd.decide(cur.right, cur.left);
				}
			}
		}
	} while (index >= 0);
}
