#include "K2PaintersAlgorithemReverse.hpp"

K2PaintersAlgorithemReverse::K2PaintersAlgorithemReverse(Vector3 viewer, int indices[]) :
	K2PaintersAlgorithem(viewer, indices)
{
  // empty
}

K2PaintersAlgorithemReverse::~K2PaintersAlgorithemReverse()
{
  // empty
}

int K2PaintersAlgorithemReverse::decide(int close, int far)
{
	mStack.push(far);
	
	return close;
}

