#include "K2PaintersAlgorithem.hpp"

#include "field.hpp"

K2PaintersAlgorithem::K2PaintersAlgorithem(Vector3 viewer, int indices[]) :
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

int K2PaintersAlgorithem::hit(int index, const Range& range)
{
	int start = range.left;
	int end = range.right;
	
	for (int q = start; q < end; q += 4)
	{
		/*
		 * the top square must always be drawn, because this function
		 * is not called if only the height of the camera changes.
		 * Fortunately it is always the first square in the array range.
		 * WARNING: be aware of this if you change the order of sqaures.
		 */
		if (q == start || dot(sgNormals[q], sub(mViewer, sgVertices[q])) >= 0)
		{
			for (int i = 0; i < 4; i++)
			{
				mIndices[mCount++] = q + i;
			}
		}
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
