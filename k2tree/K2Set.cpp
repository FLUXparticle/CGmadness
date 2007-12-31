#include "K2Set.hpp"

K2Set::K2Set(K2Tree& tree) :
	mTree(tree)
{
	mIndex = -1;
}

K2Set::~K2Set()
{
	// empty
}

int K2Set::decide(int close, int far)
{
	return close;
}

int K2Set::hit(int index, const Range& range)
{
	mIndex = index;
	return -1;
}

int K2Set::miss(int index)
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
