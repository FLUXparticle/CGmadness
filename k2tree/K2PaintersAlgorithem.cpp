#include "K2PaintersAlgorithem.hpp"

K2PaintersAlgorithem::K2PaintersAlgorithem(const K2Tree& tree, Vector3 viewer, int indices[]) :
	mTree(tree),
	mViewer(viewer),
	mIndices(indices),
	mCount(0)
{
	mStack.push(-1);
}

K2PaintersAlgorithem::~K2PaintersAlgorithem()
{
  // empty
}

int K2PaintersAlgorithem::decide(int close, int far)
{
	mStack.push(close);
	
	return far;
}

int K2PaintersAlgorithem::hit(int index)
{
	const Range& range = mTree.getRange(index);
	
	for (int i = range.left; i < range.right; i++)
	{
		mIndices[mCount++] = i;
	}
	
	return pop();
}

int K2PaintersAlgorithem::miss(int index)
{
	return pop();
}

int K2PaintersAlgorithem::pop()
{
	int index = mStack.top();
	
	mStack.pop();
	
	return index;
}
