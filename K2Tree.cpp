#include "K2Tree.hpp"

struct Range
{
	int startX;
	int startY;
	int sizeX;
	int sizeY;
	
	int left;
	int right;

	Range(int startX, int startY, int sizeX, int sizeY)
	{
		this->startX = startX;
		this->startY = startY;
		this->sizeX = sizeX;
		this->sizeY = sizeY;
		
		left = 0;
		right = 0;
	}
};

class K2Command
{
public:
	K2Command()
	{
		// empty
	}
	
	virtual ~K2Command()
	{
		// empty
	}
	
	virtual int decide(int close, int far) = 0;
	virtual int hit(int index) = 0;
	virtual int miss(int index) = 0;
	
};

class K2Get : public K2Command
{
private:
	int mIndex;
	
public:
	K2Get()
	{
		mIndex = -1;
	}
	
	virtual ~K2Get()
	{
		// empty
	}
	
	int decide(int close, int far)
	{
		return close;
	}
	
	int hit(int index)
	{
		mIndex = index;
		return -1;
	}
	
	int miss(int index)
	{
		return -1;
	}
	
	int index() const
	{
		return mIndex;
	}
	
};

class K2Set : public K2Command
{
private:
	K2Tree& mTree;
	
	int mIndex;
	
public:
	K2Set(K2Tree& tree) :
		mTree(tree)
	{
		mIndex = -1;
	}
	
	virtual ~K2Set()
	{
		// empty
	}
	
	int decide(int close, int far)
	{
		return close;
	}
	
	int hit(int index)
	{
		mIndex = index;
		return -1;
	}
	
	int miss(int index)
	{
		Range& cur = mTree.mRanges[index];
		
		Range left = cur; 
		Range right = cur; 
		
		if (cur.sizeX > cur.sizeY)
		{
			left.sizeX = cur.sizeX / 2;
			right.sizeX = cur.sizeX - left.sizeX;
			right.startX = left.startX + left.sizeX;
		}
		else
		{
			left.sizeY = cur.sizeY / 2;
			right.sizeY = cur.sizeY - left.sizeY;
			right.startY = left.startY + left.sizeY;
		}
		
		cur.left = mTree.newNode(left);
		cur.right = mTree.newNode(right);
		
		return index;
	}
	
	int index() const
	{
		return mIndex;
	}
	
};

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
