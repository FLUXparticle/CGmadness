#include "K2Tree.hpp"

K2Tree::K2Tree(int x, int y) :
	mX(x),
	mY(y)
{
	newNode();
}

K2Tree::~K2Tree()
{
  // empty
}

void K2Tree::set(int x, int y, int start, int end)
{
	int index = find(x, y);
	
	mTree[index] = start;
	mTree[index + 1] = end;
}

void K2Tree::get(int x, int y, int &start, int &end)
{
	int index = find(x, y);
	
	start = mTree[index];
	end = mTree[index + 1];
}

int K2Tree::newNode()
{
	int index = mTree.size();
		
	mTree.push_back(0);
	mTree.push_back(0);
	
	return index;
}

int K2Tree::find(int x, int y)
{
	int startX = 0;
	int startY = 0;
	int sizeX = mX;
	int sizeY = mY;
	
	int index = 0;
	
	while (true)
	{
		if (sizeX == 0 || sizeY == 0)
		{
			return -1;
		}
		else if (sizeX == 1 && sizeY == 1)
		{
			return index;
		}
		else
		{
			int startXL = startX;
			int startYL = startY;
			int sizeXL = sizeX;
			int sizeYL = sizeY;
	
			int startXR = startX;
			int startYR = startY;
			int sizeXR = sizeX;
			int sizeYR = sizeY;
			
			bool left;
	
			if (sizeX > sizeY)
			{
				sizeXL = sizeX / 2;
				sizeXR = sizeX - sizeXL;
				startXR = startXL + sizeXL;
				left = x < startXR;
			}
			else
			{
				sizeYL = sizeY / 2;
				sizeYR = sizeY - sizeYL;
				startYR = startYL + sizeYL;
				left = y < startYR;
			}
			
			int nextIndexRef;
			
			if (left)
			{
				startX = startXL;
				startY = startYL;
				sizeX = sizeXL;
				sizeY = sizeYL;
				
				nextIndexRef = index;
			}
			else
			{
				startX = startXR;
				startY = startYR;
				sizeX = sizeXR;
				sizeY = sizeYR;
				
				nextIndexRef = index + 1;
			}
			
			index = mTree[nextIndexRef];
			
			if (index == 0)
			{
				index = newNode();
				mTree[nextIndexRef] = index;
			}
		}
	}
}
